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
	SocketThread* LoginSockRun;
	FRunnableThread* LoginThread;
	PacketDispatcher* Dispatcher;
	FRunnableThread* DispatcherThread;
	TQueue<TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe>>* PacketQueue;

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	template <typename T>
	void SendPacketToLoginServer(LoginPacketListID ID, T Packet);
};
