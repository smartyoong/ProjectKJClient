// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ClientSocketThread.h"
#include "PacketDispatcher.h"
#include "CommonGameModeBase.h"
#include "PacketProcessor.h"
#include "LoginPacketList.h"
#include "JsonUtilities.h"
#include "GamePacketList.h"
#include "ResourceLoader.h"
#include "GlobalTypes.h"
#include "MainGameInstance.generated.h"
/**
 * 
 */
UCLASS()
class PROJECTKJCLIENT_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:

	UPROPERTY()

	//유저 최초 생성 관련 변수
	FCharacterInfo CharacterSpawnInfo;

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

	//리소스 변수
	TSharedPtr<ResourceLoader> Loader;
	TMap<int32,FJsonChracterPreset> ResChracterPresetMap;
	TMap<int32,FJsonMapInfo> ResMapInfoMap;

	//동기화 객체
	FCriticalSection CriticalSection;
public:

	virtual void Init() override;
	virtual void Shutdown() override;

	// 인스턴스에 현재 사용할 게임모드를 설정합니다. 이는 패킷을 받아오기에 매우 중요합니다.
	void RegistGameModeToPacketQueue(ACommonGameModeBase* GameMode);
	void SetNickName(FString NewNickName) { CharacterSpawnInfo.NickName = NewNickName;}
	void SetUserAuthHashCode(FString Code) { CharacterSpawnInfo.AuthHashCode = Code;}
	void SetAccountID(FString ID) { CharacterSpawnInfo.AccountID = ID; }
	FString GetAccountID() { return CharacterSpawnInfo.AccountID; }
	FString GetNickName() {return CharacterSpawnInfo.NickName;}
	FString GetUserAuthHashCode() {return CharacterSpawnInfo.AuthHashCode;}
	void LoadResource();

	//리소스 관련 함수들
	FString GetMapNameByMapID(int32 MapID);
	void SetCharacterPresetID(int32 ID);
	void SetFirstSpawnLocation(FVector Location);
	void SetSpawnMapID(int32 ID);
	void SetCharacterGender(int32 Gender);
	void SetCharacterJob(int32 Job);
	void SetCharacterJobLevel(int32 JobLevel);
	void SetCharacterLevel(int32 Level);
	void SetCharacterEXP(int32 EXP);
	FCharacterInfo GetCharacterSpawnInfo() { return CharacterSpawnInfo; }
	FString GetCharacterPresetBPPath(int32 ID);

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
