// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ClientSocketThread.h"
#include "PacketDispatcher.h"
#include "CommonGameModeBase.h"
#include "PacketProcessor.h"
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
	PacketProcessor* LoginPacketProcessor;
	FRunnableThread* LoginPacketProcessorThread;
	TQueue<TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe>>* LoginPacketQueue;
	TQueue<TSharedPtr<TPair<int32, TArray<uint8>>, ESPMode::ThreadSafe>>* LoginDestinationPacketQueue;

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	// 인스턴스에 현재 사용할 게임모드를 설정합니다. 이는 패킷을 받아오기에 매우 중요합니다.
	void RegistGameModeToPacketQueue(ACommonGameModeBase* GameMode);
	// 패킷을 받아오는 함수입니다.
	void UnRegistGameModeFromPacketQueue(ACommonGameModeBase* GameMode);

	template <typename T>
	void SendPacketToLoginServer(LoginPacketListID ID, T Packet);
};
