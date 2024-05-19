// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginGameModeBase.h"
#include "MyUserWidget.h"

ALoginGameModeBase::ALoginGameModeBase()
{
	LoginWidget = nullptr;
	DefaultPawnClass = nullptr;
}

void ALoginGameModeBase::BeginPlay()
{
	Super::BeginPlay();

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
