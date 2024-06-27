// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMapGameModeBase.h"
#include "WorldInfoToJson.h"
#include "MainGameInstance.h"
#include "LoadingScreenWidget.h"

void AStartMapGameModeBase::ShowLoadingScreen()
{
	if (LoadingWidget != nullptr)
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				LoadingWidget->AddToViewport();
			});
	}

}

void AStartMapGameModeBase::HideLoadingScreen()
{
	if (LoadingWidget != nullptr)
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				LoadingWidget->RemoveFromParent();
			});
	}

}

void AStartMapGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetGameInstance<UMainGameInstance>()->RegistGameModeToPacketQueue(this);

	if (GetWorld() != nullptr)
	{
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
	}

	LoadingWidget = CreateWidget<ULoadingScreenWidget>(GetWorld(), LoadingWidgetClass);
	
	// 맵 정보 읽어올때 사용
	//WorldInfoToJson::SaveWorldInfoToJson(GetWorld(), 0);
	//WorldInfoToJson::SaveWorldPortalInfoToJson(GetWorld(), 0);
}

void AStartMapGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
