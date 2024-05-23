// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoginPacketManager.generated.h"


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
    REGIST_ACCOUNT_RESPONESE = 3
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
    bool IsSuccess;
    UPROPERTY()
    int32 ErrorCode;
};

/**
 *
 */
