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
		break;
	default:
		break;
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
