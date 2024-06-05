// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginGameModeBase.h"
#include "MyUserWidget.h"
#include "ProjectKJClient/MainGameInstance.h"
#include "RegistAccountUserWidget.h"

ALoginGameModeBase::ALoginGameModeBase()
{
	LoginWidget = nullptr;
	DefaultPawnClass = nullptr;
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
		UE_LOG(LogTemp, Warning, TEXT("%s %s"),*GetWorld()->GetGameInstance<UMainGameInstance>()->GetAccountID(), *Packet.HashValue);
		// 일단 게임서버에 인증 요청 하는것부터 시작하자
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
}

void ALoginGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	LoginWidget->RemoveFromParent();
	LoginWidget->Destruct();
}
