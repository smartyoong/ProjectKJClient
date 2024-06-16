// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ClientSocketThread.h"
#include "PacketDispatcher.h"
#include "CommonGameModeBase.h"
#include "PacketProcessor.h"
#include "LoginPacketManager.h"
#include "JsonUtilities.h"
#include "Public/GamePacketList.h"
#include "MainGameInstance.generated.h"
/**
 * 
 */
UCLASS()
class PROJECTKJCLIENT_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:

	int32 UUID = -1;

	FString AccountID;
	FString NickName;
	FString AuthHashCode;

	// 로그인 관련 변수
	SocketThread* LoginSockRun;
	FRunnableThread* LoginThread;
	PacketDispatcher* LoginDispatcher;
	FRunnableThread* LoginDispatcherThread;
	PacketProcessor* LoginPacketProcessor;
	FRunnableThread* LoginPacketProcessorThread;
	TQueue<TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe>>* LoginPacketQueue;
	TQueue<TSharedPtr<TPair<int32, TArray<uint8>>, ESPMode::ThreadSafe>>* LoginDestinationPacketQueue;

	// 게임 관련 변수
	SocketThread* GameSockRun;
	FRunnableThread* GameThread;
	PacketDispatcher* GameDispatcher;
	FRunnableThread* GameDispatcherThread;
	PacketProcessor* GamePacketProcessor;
	FRunnableThread* GamePacketProcessorThread;
	TQueue<TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe>>* GamePacketQueue;
	TQueue<TSharedPtr<TPair<int32, TArray<uint8>>, ESPMode::ThreadSafe>>* GameDestinationPacketQueue;

public:

	virtual void Init() override;
	virtual void Shutdown() override;

	// 인스턴스에 현재 사용할 게임모드를 설정합니다. 이는 패킷을 받아오기에 매우 중요합니다.
	void RegistGameModeToPacketQueue(ACommonGameModeBase* GameMode);
	void SetNickName(FString NewNickName) {NickName = NewNickName;}
	void SetUserAuthHashCode(FString Code) {AuthHashCode = Code;}
	void SetAccountID(FString ID) { AccountID = ID; }
	FString GetAccountID() { return AccountID; }
	FString GetNickName() {return NickName;}
	FString GetUserAuthHashCode() {return AuthHashCode;}

	template <typename T>
	inline void SendPacketToLoginServer(LoginPacketListID ID, T Packet)
	{
		// 직렬화해서 Send 시키자 
		TArray<uint8> PacketData;
		PacketData.AddUninitialized(sizeof(int32));
		FMemory::Memcpy(PacketData.GetData(), &ID, sizeof(int32));
		FString JSonString;
		FJsonObjectConverter::UStructToJsonObjectString(Packet, JSonString);
		LoginSockRun->SendPacket(JSonString, PacketData);
	}
	template <typename T>
	inline void SendPacketToGameServer(GamePacketListID ID, T Packet)
	{
		// 직렬화해서 Send 시키자 
		TArray<uint8> PacketData;
		PacketData.AddUninitialized(sizeof(int32));
		FMemory::Memcpy(PacketData.GetData(), &ID, sizeof(int32));
		FString JSonString;
		FJsonObjectConverter::UStructToJsonObjectString(Packet, JSonString);
		GameSockRun->SendPacket(JSonString, PacketData);
	}
};
