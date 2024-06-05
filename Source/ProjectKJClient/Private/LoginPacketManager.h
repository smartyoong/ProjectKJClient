// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoginPacketManager.generated.h"

const int ERR_SQL_RETURN_ERROR = 999999;

enum class PacketProcessorMode
{
	LOGIN = 0,
	INGAME = 1
};

enum class LoginPacketListID
{
    LOGIN_REQUEST = 0,
    LOGIN_RESPONESE = 1,
    REGIST_ACCOUNT_REQUEST = 2,
    REGIST_ACCOUNT_RESPONESE = 3,
    ID_UNIQUE_CHECK_REQUEST = 4,
    ID_UNIQUE_CHECK_RESPONESE = 5
};

UENUM()
enum class ErrorCodePacket
{
	IS_NOT_USTRUCT = 0,
	FAIL_TO_DESERIALIZE = 1
};

USTRUCT()
struct FErrorPacket
{
    GENERATED_BODY()

    UPROPERTY()
    ErrorCodePacket ErrorCode;
};

USTRUCT()
struct FLoginRequestPacket
{
    GENERATED_BODY()

    UPROPERTY()
    FString AccountID;
    UPROPERTY()
    FString Password;
};

USTRUCT()
struct FLoginResponsePacket
{
    GENERATED_BODY()

    UPROPERTY()
    FString NickName;
    UPROPERTY()
    FString HashValue;
    UPROPERTY()
    int32 ErrorCode;
};
USTRUCT()
struct FRegistAccountRequestPacket
{
    GENERATED_BODY()

    UPROPERTY()
    FString AccountID;
    UPROPERTY()
    FString Password;
};
USTRUCT()
struct FRegistAccountResponsePacket
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ErrorCode;
};

USTRUCT()
struct FIDUniqueCheckRequestPacket
{
	GENERATED_BODY()

	UPROPERTY()
	FString AccountID;
};

USTRUCT()
struct FIDUniqueCheckResponsePacket
{
	GENERATED_BODY()

	UPROPERTY()
	bool IsUnique;
};

/**
 *
 */
