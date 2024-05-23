// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ClientSocketThread.h"
#include "PacketDispatcher.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKJCLIENT_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	// 로그인 관련 변수
	SocketThread* LoginSockRun;
	FRunnableThread* LoginThread;
	PacketDispatcher* LoginDispatcher;
	FRunnableThread* LoginDispatcherThread;
	TQueue<TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe>>* LoginPacketQueue;
	TQueue<TSharedPtr<TPair<int32, TArray<uint8>>, ESPMode::ThreadSafe>>* LoginDestinationPacketQueue;

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	template <typename T>
	void SendPacketToLoginServer(LoginPacketListID ID, T Packet);
};
