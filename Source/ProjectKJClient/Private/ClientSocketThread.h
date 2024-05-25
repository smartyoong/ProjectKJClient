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

	void SendPacket(const FString& JsonString, TArray<uint8>& PacketData)
	{
		// 사이즈를 지정해야하므로 중간 개체를 설정하자 1만 바이트로 우선,,,
		TArray<uint8> data;
		data.AddUninitialized(10000);
		int32 Size = StringToBytes(JsonString, data.GetData(), data.Num());
		// 사이즈를 다시 설정하자
		TArray<uint8> FinalData;
		FinalData.AddUninitialized(Size);
		FMemory::Memcpy(FinalData.GetData(), data.GetData(), Size);
		PacketData.Append(FinalData);
		Socket.SendToServer(PacketData.GetData(), PacketData.Num());
	}

private:
    int Mode = 0;
    FThreadSafeCounter StopTaskCounter=0;
    ClientSocket Socket;
    bool InitSuccess = false;
    TQueue<TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe>>* PacketQueue;
};
