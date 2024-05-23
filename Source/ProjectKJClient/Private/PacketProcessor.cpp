// Fill out your copyright notice in the Description page of Project Settings.


#include "PacketProcessor.h"
#include "JsonUtilities.h"

PacketProcessor::PacketProcessor(TQueue<TSharedPtr<TPair<int32, TArray<uint8>>, ESPMode::ThreadSafe>>* Queue, 
	PacketProcessorMode mode) : PacketQueue(Queue), Mode(mode), StopTaskCounter(0)
{
}

PacketProcessor::~PacketProcessor()
{
}

bool PacketProcessor::Init()
{
	if(PacketQueue == nullptr)
		return false;
	return true;
}

uint32 PacketProcessor::Run()
{
	while(!StopTaskCounter.GetValue())
	{
		TSharedPtr<TPair<int32, TArray<uint8>>> PacketData;
		if (PacketQueue->Dequeue(PacketData))
		{
			if (!PacketData.IsValid())
				continue;
			// 패킷 처리
			switch (Mode)
			{
				case PacketProcessorMode::LOGIN:
					ProcessLoginPacket(PacketData);
					break;
				case PacketProcessorMode::INGAME:
					ProcessGamePacket(PacketData);
					break;
				default:
					UE_LOG(LogTemp, Error, TEXT("Invalid PacketProcessor Mode"));
					break;
			}
		}
	}
	return 0;
}
template <typename T>
T PacketProcessor::PacketToStruct(const TArray<uint8>& Data)
{
	FString String = BytesToString(Data);
	T PacketStruckt;
	FJsonObjectConverter::JsonObjectStringToUStruct(String, &PacketStruckt, 0, 0);
	return PacketStruckt;
}

void PacketProcessor::Stop()
{
	StopTaskCounter.Increment();
}

void PacketProcessor::Exit()
{
}

void PacketProcessor::ProcessLoginPacket(TSharedPtr<TPair<int32, TArray<uint8>>> PacketData)
{
	LoginPacketListID ID = static_cast<LoginPacketListID>(PacketData->Key);
	switch (ID)
	{
		case LoginPacketListID::LOGIN_REQUEST:
			break;
		case LoginPacketListID::LOGIN_RESPONESE:
			break;
		case LoginPacketListID::REGIST_ACCOUNT_REQUEST:
			break;
		case LoginPacketListID::REGIST_ACCOUNT_RESPONESE:
			break;
		default:
			break;
			// 패킷을 진짜로 처리하고 GameModeBase를 장착시키고 인스턴스에서 이 스레드를 실행시키자
	}
}


void PacketProcessor::ProcessGamePacket(TSharedPtr<TPair<int32, TArray<uint8>>> PacketData)
{

}
