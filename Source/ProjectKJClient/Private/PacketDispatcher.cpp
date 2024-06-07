// Fill out your copyright notice in the Description page of Project Settings.


#include "PacketDispatcher.h"

int32 PacketDispatcher::GetIDFromPacket(TSharedPtr<TArray<uint8>> PacketData)
{
	int32 PacketID = 0;
	FMemory::Memcpy(&PacketID, PacketData->GetData(), sizeof(int32));
	PacketData->RemoveAt(0, sizeof(int32));
	return PacketID;
}

PacketDispatcher::PacketDispatcher(TQueue<TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe>>* Queue, 
	TQueue<TSharedPtr<TPair<int32, TArray<uint8>>, ESPMode::ThreadSafe>>* Dest) : PacketQueue(Queue), DestinationQueue(Dest)
{
}

bool PacketDispatcher::Init()
{
	if (PacketQueue == nullptr or DestinationQueue == nullptr)
	{
		return false;
	}
	return true;
}

uint32 PacketDispatcher::Run()
{
	if (PacketQueue == nullptr)
		return -1;
	while (!StopTaskCounter.GetValue())
	{
		TSharedPtr<TArray<uint8>> PacketData;
		if (PacketQueue != nullptr && PacketQueue->Dequeue(PacketData))
		{
			if (!PacketData.IsValid())
				continue;

			int32 PacketID = GetIDFromPacket(PacketData);
			// 이제 로그인 큐 혹은 게임 큐에 넣자
			TSharedPtr<TPair<int32, TArray<uint8>>> PacketPair = MakeShareable(new TPair<int32, TArray<uint8>>(PacketID, *PacketData));
			DestinationQueue->Enqueue(PacketPair);
		}
	}
	return 0;
}

void PacketDispatcher::Stop()
{
	StopTaskCounter.Increment();
}

void PacketDispatcher::Exit()
{
	UE_LOG(LogTemp, Warning, TEXT("PacketDispatcher Exit"));
}


