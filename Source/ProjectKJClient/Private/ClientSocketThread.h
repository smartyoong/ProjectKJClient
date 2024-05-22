// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClientSocket.h"
#include "LoginPacketManager.h"

/**
 * 
 */
const int LOGIN_MODE = 0;
const int GAME_MODE = 1;

class SocketThread : public FRunnable
{
public:
	SocketThread(int mode ,TQueue<TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe>>* InPacketQueue);

    virtual bool Init() override;

    virtual uint32 Run() override;

    virtual void Stop() override;

    virtual void Exit() override;

	bool GetInitSuccess() const { return InitSuccess; }

    template <typename T>
    void SendPacket(LoginPacketListID ID, T Packet);

private:
    int Mode = 0;
    FThreadSafeCounter StopTaskCounter;
    ClientSocket Socket;
    bool InitSuccess = false;
    TQueue<TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe>>* PacketQueue;
};
