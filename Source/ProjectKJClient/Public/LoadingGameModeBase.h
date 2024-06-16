// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonGameModeBase.h"
#include "LoadingGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKJCLIENT_API ALoadingGameModeBase : public ACommonGameModeBase
{
	GENERATED_BODY()

private:
	FString DestinationLevelName;
	UPROPERTY()
	class ULoadingScreenWidget* LoadingWidget;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class ULoadingScreenWidget> LoadingWidgetClass;
	void ShowLoadingScreenAndOpenLevel(FString LevelName);
	void OnLoadingLevelLoadCompleted();
};
