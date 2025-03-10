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
    REQUEST_CREATE_CHARACTER = 8,
    RESPONSE_CREATE_CHARACTER = 9,
    REQUEST_MOVE = 10,
    RESPONSE_MOVE = 11,
	SEND_ANOTHER_CHAR_BASE_INFO = 12,
    REQUEST_GET_SAME_MAP_USER = 13,
	SEND_USER_MOVE = 14,
    REQUEST_PING_CHECK = 15,
	RESPONSE_PING_CHECK = 16,
    SEND_USER_MOVE_ARRIVED = 17,
	SEND_USER_SAY = 18,
	REQUEST_USER_SAY = 19,
    SEND_LEVEL_UP = 20,
    SEND_JOB_CHANGE = 21,
    SEND_GENDER_CHANGE = 22,
    SEND_PRESET_CHANGE = 23
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
    UPROPERTY()
    FString NickName;
};

USTRUCT()
struct FResponseCharBaseInfoPacket
{
    GENERATED_BODY()

    UPROPERTY()
    FString AccountID;
    UPROPERTY()
    int32 Gender;
    UPROPERTY()
    int32 PresetNumber;
    UPROPERTY()
    int32 Job;
    UPROPERTY()
    int32 JobLevel;
    UPROPERTY()
    int32 MapID;
    UPROPERTY()
    int32 X;
    UPROPERTY()
    int32 Y;
    UPROPERTY()
    int32 Level;
    UPROPERTY()
    int32 EXP;
	UPROPERTY()
    int32 HP;
	UPROPERTY()
	int32 MP;
};

USTRUCT()
struct FResponseNeedToMakeCharcterPacket
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ErrorCode;
};

USTRUCT()
struct FRequestCreateCharacterPacket
{
    GENERATED_BODY()

    UPROPERTY()
    FString AccountID;
    UPROPERTY()
    FString HashCode;
    UPROPERTY()
    int32 Gender;
    UPROPERTY()
    int32 PresetID;
};

USTRUCT()
struct FResponseCreateCharacterPacket
{
	GENERATED_BODY()

	UPROPERTY()
	int32 ErrorCode;
};

USTRUCT()
struct FRequestMovePacket
{
    GENERATED_BODY()

    UPROPERTY()
	FString AccountID;
    UPROPERTY()
	FString HashCode;
    UPROPERTY()
	int32 MapID;
	UPROPERTY()
	int32 X;
	UPROPERTY()
	int32 Y;
};

USTRUCT()
struct FResponseMovePacket
{
	GENERATED_BODY()

	UPROPERTY()
	int32 ErrorCode;
};

USTRUCT()
struct FSendAnotherCharBaseInfoPacket
{
    GENERATED_BODY()

    UPROPERTY()
    FString AccountID;
    UPROPERTY()
    int32 Gender;
    UPROPERTY()
    int32 PresetNumber;
    UPROPERTY()
    int32 Job;
    UPROPERTY()
    int32 JobLevel;
    UPROPERTY()
    int32 MapID;
    UPROPERTY()
    int32 X;
    UPROPERTY()
    int32 Y;
    UPROPERTY()
    int32 Level;
    UPROPERTY()
    int32 EXP;
	UPROPERTY()
	FString NickName;
    UPROPERTY()
    int32 DestX;
	UPROPERTY()
	int32 DestY;
    UPROPERTY()
	int32 HP;
	UPROPERTY()
	int32 MP;
};

USTRUCT()
struct FRequestGetSameMapUserPacket
{
    GENERATED_BODY()

    UPROPERTY()
	FString AccountID;
	UPROPERTY()
	FString HashCode;
	UPROPERTY()
	int32 MapID;
};

USTRUCT()
struct FSendUserMovePacket
{
	GENERATED_BODY()

	UPROPERTY()
	FString AccountID;
	UPROPERTY()
	int32 MapID;
	UPROPERTY()
	int32 X;
	UPROPERTY()
	int32 Y;
};

USTRUCT()
struct FRequestPingCheckPacket
{
	GENERATED_BODY()

	UPROPERTY()
    int32 Hour;

	UPROPERTY()
	int32 Min;

	UPROPERTY()
	int32 Secs;

	UPROPERTY()
	int32 MSecs;
};

USTRUCT()
struct FResponsePingCheckPacket
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Hour;

	UPROPERTY()
	int32 Min;

	UPROPERTY()
	int32 Secs;

	UPROPERTY()
	int32 MSecs;
};

USTRUCT()
struct FSendUserMoveArrivedPacket
{
	GENERATED_BODY()

	UPROPERTY()
	FString AccountID;
    UPROPERTY()
    int32 MapID;
	UPROPERTY()
	int32 X;
	UPROPERTY()
	int32 Y;
};

USTRUCT()
struct FSendUserSayPacket
{
	GENERATED_BODY()

	UPROPERTY()
	FString AccountID;
    UPROPERTY()
    FString SenderNickName;
	UPROPERTY()
	FString Message;
    UPROPERTY()
	int32 SayType;
};

USTRUCT()
struct FRequestUserSayPacket
{
	GENERATED_BODY()
	UPROPERTY()
	FString AccountID;
	UPROPERTY()
	FString Message;
};

USTRUCT()
struct FSendLevelUpPacket
{
    GENERATED_BODY()
    UPROPERTY()
    FString AccountID;
    UPROPERTY()
    int32 Level;
    UPROPERTY()
    int32 EXP;
};

USTRUCT()
struct FSendJobChangePacket
{
    GENERATED_BODY()
    UPROPERTY()
    FString AccountID;
    UPROPERTY()
    int32 Job;
    UPROPERTY()
    int32 JobLevel;
};

USTRUCT()
struct FSendGenderChangePacket
{
    GENERATED_BODY()
    UPROPERTY()
    FString AccountID;
    UPROPERTY()
    int32 GenderType;
};

USTRUCT()
struct FSendPresetChangePacket
{
    GENERATED_BODY()
    UPROPERTY()
    FString AccountID;
    UPROPERTY()
    int32 PresetNumber;
};