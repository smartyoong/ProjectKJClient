// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GlobalTypes.generated.h"

/**
 * 
 */
USTRUCT()
struct FJsonChracterPreset
{
	GENERATED_BODY()

	UPROPERTY()
	int32 PresetID;
	UPROPERTY()
	FString PresetName;
	UPROPERTY()
	int32 Gender;
	UPROPERTY()
	FString BlueprintName;
	UPROPERTY()
	FString PlayerCharacterName;
};

USTRUCT()
struct FJsonObstacle
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FVector Scale;

	UPROPERTY()
	FVector MeshSize;

	UPROPERTY()
	FString MeshName;
};

USTRUCT()
struct FJsonMapInfo
{
	GENERATED_BODY()

	UPROPERTY()
	int32 MapID;

	UPROPERTY()
	FString MapName;

	UPROPERTY()
	TArray<FJsonObstacle> Obstacles;
};
