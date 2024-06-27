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
	KICK_CLIENT = 4,
    REQUEST_CHAR_BASE_INFO = 5,
    RESPONSE_NEED_TO_MAKE_CHARACTER = 6,
    RESPONSE_CHAR_BASE_INFO = 7,
    REQUEST_CREATE_CHARACTER = 8
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

USTRUCT()
struct FRequestCharBaseInfoPacket
{
    GENERATED_BODY()

    UPROPERTY()
    FString AccountID;
    UPROPERTY()
    FString HashCode;
};

USTRUCT()
struct FResponseCharBaseInfoPacket
{
    GENERATED_BODY()

    UPROPERTY()
    int32 Level;
    UPROPERTY()
    int32 Exp;
    UPROPERTY()
    int32 Job;
    UPROPERTY()
    int32 JobLevel;
    UPROPERTY()
    int32 LastMapID;
    UPROPERTY()
    int32 LastMapX;
    UPROPERTY()
    int32 LastMapY;
};

USTRUCT()
struct FResponseNeedToMakeCharcterPacket
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ErrorCode;
};

USTRUCT()
struct FRequestCreateCharacter
{
    GENERATED_BODY()

    UPROPERTY()
    FString AccountID;
    UPROPERTY()
    FString HashCode;
    UPROPERTY()
    FString NickName;
    UPROPERTY()
    int32 Gender;
};
