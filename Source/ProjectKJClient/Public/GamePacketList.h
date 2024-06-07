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
    RESPONSE_HASH_AUTH_CHECK = 3,
	KICK_CLIENT = 4
};

enum class KickReason
{
	REASON_DUPLICATE_LOGIN = 0
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

USTRUCT()
struct FSendKickClientPacket
{
    GENERATED_BODY()

    UPROPERTY()
    int32 Reason;
};
