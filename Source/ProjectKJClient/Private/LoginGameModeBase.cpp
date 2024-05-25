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
	case 0:
		UE_LOG(LogTemp, Warning, TEXT("No Account"));
		break;
	case 1:
		UE_LOG(LogTemp, Warning, TEXT("Password Failed"));
		break;
	case 2:
		UE_LOG(LogTemp, Warning, TEXT("Login Success"));
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
	GetWorld()->GetGameInstance<UMainGameInstance>()->UnRegistGameModeFromPacketQueue();
}
