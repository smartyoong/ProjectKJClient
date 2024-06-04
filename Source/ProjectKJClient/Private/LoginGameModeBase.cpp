// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginGameModeBase.h"
#include "MyUserWidget.h"
#include "ProjectKJClient/MainGameInstance.h"

ALoginGameModeBase::ALoginGameModeBase()
{
	LoginWidget = nullptr;
	DefaultPawnClass = nullptr;
}

void ALoginGameModeBase::OnLoginResponsePacketReceived(FLoginResponsePacket Packet)
{
	switch (Packet.ErrorCode)
	{
	case 0: // 계정 없음
		LoginWidget->ShowLoginResultWidget(Packet.ErrorCode);
		break;
	case 1: // 비번 불일치
		LoginWidget->ShowLoginResultWidget(Packet.ErrorCode);
		break;
	case 2: // 성공
		LoginWidget->ShowLoginResultWidget(Packet.ErrorCode);

		// 이제 게임서버와 Connect 시키는 거부터 작업하고 (GameInstance랑 ClientSocket, Packet Dispatcher랑 Processor 수정필요)
		// 맵을 이동하는 것을 구현하도록 하자

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
