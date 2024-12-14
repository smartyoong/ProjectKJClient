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

USTRUCT()
struct FCharacterInfo
{
	GENERATED_BODY()
	//유저 캐릭터 관련 변수 (메인 인스턴스가 들고 있음) 맵 이동시에 들고 있어야할 정보들
	UPROPERTY()
	FString AccountID;
	UPROPERTY()
	FString NickName;
	UPROPERTY()
	FString AuthHashCode;
	UPROPERTY()
	int32 CharacterPresetID;
	UPROPERTY()
	FVector FirstSpawnLocation;
	UPROPERTY()
	int32 SpawnMapID;
	UPROPERTY()
	int32 Gender;
	UPROPERTY()
	int32 Job;
	UPROPERTY()
	int32 JobLevel;
	UPROPERTY()
	int32 Level;
	UPROPERTY()
	int32 EXP;
	UPROPERTY()
	int32 HP;
	UPROPERTY()
	int32 MP;
};
