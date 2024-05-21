// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


enum class LoginPacketListID
{
    LOGIN_REQUEST = 0,
    LOGIN_RESPONESE = 1,
    REGIST_ACCOUNT_REQUEST = 2,
    REGIST_ACCOUNT_RESPONESE = 3
};

struct LoginRequestPacket
{
    FString AccountID;
    FString Password;
};

struct LoginResponsePacket
{
    FString NickName;
    int32 ErrorCode;
};

struct RegistAccountRequestPacket
{
    FString AccountID;
    FString Password;
};

struct RegistAccountResponsePacket
{
    bool IsSuccess;
    int32 ErrorCode;
};

/**
 *
 */


class PacketDispatcher
{
private:
	TQueue<uint8*>* PacketQueue;
	TQueue<uint8*> LoginPacketQueue;
	TQueue<uint8*> GamePacketQueue;

public:
    PacketDispatcher(TQueue<uint8*>* Queue);
    ~PacketDispatcher() = default;
};
