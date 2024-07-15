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

void ALoginGameModeBase::SendRequestCharBaseInfoPacket()
{
	FRequestCharBaseInfoPacket RequestCharBaseInfoPacket;
	if(!IsValid(GetWorld()->GetGameInstance<UMainGameInstance>()))
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance is not valid send request char base info packet"));
		return;
	}
	if(GetWorld()->GetGameInstance<UMainGameInstance>()->GetAccountID().IsEmpty() || GetWorld()->GetGameInstance<UMainGameInstance>()->GetUserAuthHashCode().IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("AccountID or AuthHashCode is not valid send request char base info packet"));
		return;
	}
	RequestCharBaseInfoPacket.AccountID = GetWorld()->GetGameInstance<UMainGameInstance>()->GetAccountID();
	RequestCharBaseInfoPacket.HashCode = GetWorld()->GetGameInstance<UMainGameInstance>()->GetUserAuthHashCode();
	GetWorld()->GetGameInstance<UMainGameInstance>()->SendPacketToGameServer<FRequestCharBaseInfoPacket>(GamePacketListID::REQUEST_CHAR_BASE_INFO, RequestCharBaseInfoPacket);
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
				SendRequestCharBaseInfoPacket();
				break;
			case AUTH_FAIL: // 인증 실패
				// 이게 서버에서 이미 접속된 유저 삭제하는 것보다 새로 등록 요청 패킷이 먼저오면 인증 실패가 나옴 그래서 재시도
				TimerManager.SetTimer(UnusedHandle, this, &ALoginGameModeBase::SendHashAuthCheckPacket, 2.0f, false);
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
}

void ALoginGameModeBase::OnResponseNeedToMakeCharacter(FResponseNeedToMakeCharcterPacket Packet)
{
	// 캐릭터 생성 팝업을 띄우자
	ShowCreateCharacterWidget();
}

void ALoginGameModeBase::OnResponseCreateCharacter(FResponseCreateCharacterPacket Packet)
{
	// 여기까지는 다른 스레드에서 실행되지만, 밑에 메서드를 통해 MainThread에서 실행되도록 한다.
	const int SUCCESS = 0;
	if (Packet.ErrorCode == SUCCESS)
	{
		if (CreateCharacterWidget != nullptr)
		{
			CreateCharacterWidget->CreateCharcterSuccess();
		}
	}
	else
	{
		if(CreateCharacterWidget != nullptr)
		{
			CreateCharacterWidget->CreateCharacterFail();
		}
	}
}

void ALoginGameModeBase::OnResponseCreateNickName(FCreateNickNameResponsePacket Packet)
{
	// 근데 캐릭 생성은 성공하고 닉네임은 실패하면 어카지?
	// 그냥 넘길까? 일단 그냥 넘기자
	// 여기까지는 다른 스레드에서 실행되지만, 밑에 메서드를 통해 MainThread에서 실행되도록 한다.
	const int SUCCESS = 0;
	if(Packet.ErrorCode != SUCCESS)
	{
		UE_LOG(LogTemp, Error, TEXT("Create NickName Fail"));
		Cast<UMainGameInstance>(GetGameInstance())->SetNickName("GUEST");
	}

	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			ShowLoadingScreen();
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("TopDownMap"));
		});
}

void ALoginGameModeBase::OnResponseCharBaseInfo(FResponseCharBaseInfoPacket Packet)
{
	// 이제 캐릭터 클래스에 정보를 장착해야하지만, 아직 캐릭터 작업 전이므로
	// 우선은 로그만 찍고 로딩화면 -> OpenMap으로 시키자.
	ShowLoadingScreen();

	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("TopDownMap"));
		});
}

void ALoginGameModeBase::ShowCreateCharacterWidget()
{
	CreateCharacterWidget = CreateWidget<UCreateCharacterUserWidget>(GetWorld(), CreateCharacterWidgetClass);
	if (CreateCharacterWidget != nullptr)
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				CreateCharacterWidget->AddToViewport();
				CreateCharacterWidget->PopulateList();
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
