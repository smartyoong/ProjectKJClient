// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonGameModeBase.h"
#include "WorldInfoToJson.h"
#include "StartMapGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKJCLIENT_API AStartMapGameModeBase : public ACommonGameModeBase
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
