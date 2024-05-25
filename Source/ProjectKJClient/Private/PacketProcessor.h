// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonGameModeBase.h"
#include "LoginPacketManager.h"
#include "JsonUtilities.h"

/**
 * 
 */
class PacketProcessor : public FRunnable
{
private:
	TQueue<TSharedPtr<TPair<int32, TArray<uint8>>, ESPMode::ThreadSafe>>* PacketQueue;
	ACommonGameModeBase* GameMode = nullptr;
	PacketProcessorMode ProcessMode;
	FThreadSafeCounter StopTaskCounter;
	FEvent* GameModeEvent;
public:
	PacketProcessor(TQueue<TSharedPtr<TPair<int32, TArray<uint8>>, ESPMode::ThreadSafe>>* Queue, PacketProcessorMode mode);
	~PacketProcessor();

	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;

	void ProcessLoginPacket(TSharedPtr<TPair<int32, TArray<uint8>>> PacketData);

	void ProcessGamePacket(TSharedPtr<TPair<int32, TArray<uint8>>> PacketData);

	void SetGameMode(ACommonGameModeBase* Mode);

	void RemoveGameMode();
	
	template <typename T>
	inline T PacketToStruct(const TArray<uint8>& Data)
	{
		FString String = BytesToString(Data.GetData(), Data.Num());
		T PacketStruct;
		if (FJsonObjectConverter::JsonObjectStringToUStruct(String, &PacketStruct, 0, 0))
			return PacketStruct;
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Fail to Deserialize Packet"));
			return PacketStruct;
		}
	}
};
