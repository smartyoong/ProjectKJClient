// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectKJClient/MainGameInstance.h"
#include "LoadingScreenWidget.h"

void ALoadingGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetGameInstance<UMainGameInstance>()->RegistGameModeToPacketQueue(this);
	if (LoadingWidgetClass)
	{
		LoadingWidget = CreateWidget<ULoadingScreenWidget>(GetWorld(), LoadingWidgetClass);
	}
	ShowLoadingScreenAndOpenLevel(TEXT("D:/ProjectKJ/GameClient/ProjectKJClient/Content/TopDown/Maps/TopDownMap.umap"));
}

void ALoadingGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (LoadingWidget)
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				LoadingWidget->RemoveFromParent();
			});
	}

}

void ALoadingGameModeBase::ShowLoadingScreenAndOpenLevel(FString LevelName)
{
	if (LoadingWidget)
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				LoadingWidget->AddToViewport();
			});
	}
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("OnLoadingLevelLoadCompleted");
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = 0;// GetWorld()->GetGameInstance<UMainGameInstance>()->GetUUID();
	DestinationLevelName = LevelName;
	UGameplayStatics::LoadStreamLevel(this, *LevelName, true, true, LatentInfo);
}

void ALoadingGameModeBase::OnLoadingLevelLoadCompleted()
{
	UGameplayStatics::UnloadStreamLevel(this, TEXT("LoadingMap"), FLatentActionInfo(), false);
	UGameplayStatics::OpenLevel(this, *DestinationLevelName);
}
