// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginGameModeBase.h"
#include "MyUserWidget.h"
#include "MainGameInstance.h"
#include "RegistAccountUserWidget.h"
#include "GamePacketList.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "LoadingScreenWidget.h"
#include "CreateCharacterUserWidget.h"

ALoginGameModeBase::ALoginGameModeBase()
{
	LoginWidget = nullptr;
	DefaultPawnClass = nullptr;
}

void ALoginGameModeBase::ShowLoadingScreen()
{
	if (LoadingWidget != nullptr)
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				LoadingWidget->AddToViewport();
			});
	}
}

void ALoginGameModeBase::HideLoadingScreen()
{
	if (LoadingWidget != nullptr)
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				LoadingWidget->RemoveFromParent();
			});
	}
}

void ALoginGameModeBase::SendHashAuthCheckPacket()
{
	GetWorld()->GetGameInstance<UMainGameInstance>()->SendPacketToGameServer(GamePacketListID::REQUEST_HASH_AUTH_CHECK,
		FRequestHashAuthCheckPacket{ GetWorld()->GetGameInstance<UMainGameInstance>()->GetAccountID(), GetWorld()->GetGameInstance<UMainGameInstance>()->GetUserAuthHashCode()});
}

void ALoginGameModeBase::OnLoginResponsePacketReceived(FLoginResponsePacket Packet)
{
	enum LoginResponseErrCode { NO_ACCOUNT = 0, PASSWORD_NOT_MATCH = 1, LOGIN_SUCCESS = 2, HASH_CODE_CREATE_FAIL = 3, ERR_AUTH_FAIL = 5, ERR_AUTH_RETRY = 6 };
	switch (Packet.ErrorCode)
	{
	case NO_ACCOUNT: // 계정 없음
		LoginWidget->ShowLoginResultWidget(Packet.ErrorCode);
		break;
	case PASSWORD_NOT_MATCH: // 비번 불일치
		LoginWidget->ShowLoginResultWidget(Packet.ErrorCode);
		break;
	case LOGIN_SUCCESS: // 성공
		LoginWidget->ShowLoginResultWidget(Packet.ErrorCode);
		GetWorld()->GetGameInstance<UMainGameInstance>()->SetNickName(Packet.NickName);
		GetWorld()->GetGameInstance<UMainGameInstance>()->SetUserAuthHashCode(Packet.HashValue);
		// 이제 게임서버와 Connect 시키는 거부터 작업하고 (GameInstance랑 ClientSocket, Packet Dispatcher랑 Processor 수정필요)
		//UE_LOG(LogTemp, Warning, TEXT("%s %s"),*GetWorld()->GetGameInstance<UMainGameInstance>()->GetAccountID(), *Packet.HashValue);
		// 일단 게임서버에 인증 요청 하는것부터 시작하자
		GetWorld()->GetGameInstance<UMainGameInstance>()->SendPacketToGameServer(GamePacketListID::REQUEST_HASH_AUTH_CHECK,
			FRequestHashAuthCheckPacket{GetWorld()->GetGameInstance<UMainGameInstance>()->GetAccountID(), Packet.HashValue});

		// 맵을 이동하는 것을 구현하도록 하자
		break;
		// 로그인은 성공했으나 Hash값 생성 실패
	case HASH_CODE_CREATE_FAIL:
		LoginWidget->ShowLoginResultWidget(Packet.ErrorCode);
		break;
		// 게임서버가 해시값을 생성 못해서 로그인 서버가 해시값 재생성 시도했으나 실패
	case ERR_AUTH_FAIL:
		LoginWidget->ShowLoginResultWidget(Packet.ErrorCode);
		break;
		// 게임서버가 해시값을 생성 못해서 로그인 서버가 해시값을 재생성해서 전송함
	case ERR_AUTH_RETRY:
		LoginWidget->ShowLoginResultWidget(Packet.ErrorCode);
		break;
	default:
		break;
	}
}

void ALoginGameModeBase::OnIDUnqiueCheckResponsePacketReceived(FIDUniqueCheckResponsePacket Packet)
{
	if(LoginWidget == nullptr)
		return;

	if(Packet.IsUnique)
	{
		if(LoginWidget->GetRegistAccountWidget() != nullptr)
			LoginWidget->GetRegistAccountWidget()->ShowIDIsUnique();
	}
	else
	{
		if(LoginWidget->GetRegistAccountWidget() != nullptr)
			LoginWidget->GetRegistAccountWidget()->ShowIDIsNotUnique();
	}

}

void ALoginGameModeBase::OnRegistAccountResponsePacketReceived(FRegistAccountResponsePacket Packet)
{
	if(LoginWidget == nullptr)
		return;

	if(LoginWidget->GetRegistAccountWidget() == nullptr)
		return;

	if(Packet.ErrorCode)
		LoginWidget->GetRegistAccountWidget()->ShowRegistFail(Packet.ErrorCode);
	else
	{
		LoginWidget->ShowWidgetItems();
		LoginWidget->GetRegistAccountWidget()->ShowRegistSuccess();
		LoginWidget->ShowRegistSuccessPopUp();
	}
}

void ALoginGameModeBase::OnHashAuthCheckResponsePacketReceived(FResponseHashAuthCheckPacket Packet)
{
	const int REGIST_NOT_YET = 2;
	const int AUTH_SUCCESS = 4;
	const int AUTH_FAIL = 5;
	AsyncTask(ENamedThreads::GameThread, [this, Packet]()
		{
			FTimerManager& TimerManager = GetWorld()->GetTimerManager();
			FTimerHandle UnusedHandle;

			switch (Packet.ErrorCode)
			{
			case REGIST_NOT_YET: // 게임 서버에 아직 등록 안됨
				TimerManager.SetTimer(UnusedHandle, this, &ALoginGameModeBase::SendHashAuthCheckPacket, 2.0f, false);
				break;
			case AUTH_SUCCESS: // 인증 성공
				ShowLoadingScreen();
				//여기 하드코딩된거 빼자
				//여기서 이제 서버한테 캐릭터 정보 요청 보내야함
				//UGameplayStatics::OpenLevel(this, TEXT("TopDownMap"), true);
				HideLoadingScreen();
				ShowCreateCharacterWidget();
				break;
			case AUTH_FAIL: // 인증 실패
				// 이게 서버에서 이미 접속된 유저 삭제하는 것보다 새로 등록 요청 패킷이 먼저오면 인증 실패가 나옴 그래서 재시도
				TimerManager.SetTimer(UnusedHandle, this, &ALoginGameModeBase::SendHashAuthCheckPacket, 2.0f, false);
				//if (LoginWidget != nullptr)
				//	LoginWidget->ShowLoginFail();
				break;
			default:
				break;
			}
		});
}

void ALoginGameModeBase::OnKickClientPacketReceived(FSendKickClientPacket Packet)
{
	// 팝업을 띄우자
	if(LoginWidget != nullptr)
		LoginWidget->ShowKickClient(Packet.Reason);
	//UE_LOG(LogTemp, Warning, TEXT("%d"), Packet.Reason);
	//UE_LOG(LogTemp, Warning, TEXT("강제 추방 당함"));
}

void ALoginGameModeBase::ShowCreateCharacterWidget()
{
	CreateCharacterWidget = CreateWidget<UCreateCharacterUserWidget>(GetWorld(), CreateCharacterWidgetClass);
	if (CreateCharacterWidget != nullptr)
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				CreateCharacterWidget->AddToViewport();
			});
	}
}

void ALoginGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetGameInstance<UMainGameInstance>()->RegistGameModeToPacketQueue(this);

	if (GetWorld() != nullptr)
	{
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	}

	LoginWidget = CreateWidget<UMyUserWidget>(GetWorld(), LoginWidgetClass);
	if (LoginWidget != nullptr)
	{
		LoginWidget->AddToViewport();
	}

	LoadingWidget = CreateWidget<ULoadingScreenWidget>(GetWorld(), LoadingWidgetClass);
}

void ALoginGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	LoginWidget->RemoveFromParent();
	LoginWidget->Destruct();
}
