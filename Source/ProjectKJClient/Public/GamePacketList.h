// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePacketList.generated.h"

/**
 * 
 */

enum class GamePacketListID
{
    REQUEST_GAME_TEST = 0,
    RESPONSE_GAME_TEST = 1,
    REQUEST_HASH_AUTH_CHECK = 2,
    RESPONSE_HASH_AUTH_CHECK = 3
};

USTRUCT()
struct FRequestHashAuthCheckPacket
{
    GENERATED_BODY()

    UPROPERTY()
    FString AccountID;
	UPROPERTY()
    FString HashCode;
};

USTRUCT()
struct FResponseHashAuthCheckPacket
{
    GENERATED_BODY()

	UPROPERTY()
    FString AccountID;
    UPROPERTY()
    int32 ErrorCode;
};
