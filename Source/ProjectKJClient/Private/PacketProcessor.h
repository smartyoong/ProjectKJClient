// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonGameModeBase.h"
#include "LoginPacketManager.h"
#include "JsonUtilities.h"
#include "GamePacketList.h"


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
	
	template <typename T>
	inline T PacketToStruct(const TArray<uint8>& Data)
	{
		//FString String = BytesToString(Data.GetData(), Data.Num());
		FString Result;
		int32 Count = Data.Num();
		Result.Empty(Count);
		const uint8* In = Data.GetData();
		while (Count)
		{
			int16 Value = *In;
			Result += FString::ElementType(Value);
			++In;
			Count--;
		}
		T PacketStruct;
		if (FJsonObjectConverter::JsonObjectStringToUStruct(Result, &PacketStruct, 0, 0))
			return PacketStruct;
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Fail to Deserialize Packet"));
			return PacketStruct;
		}
	}
};
