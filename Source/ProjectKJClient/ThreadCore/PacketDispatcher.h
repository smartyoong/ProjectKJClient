// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PacketDispatcher : public FRunnable
{
private:
	TQueue<TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe>>* PacketQueue;
	TQueue<TSharedPtr<TPair<int32,TArray<uint8>>, ESPMode::ThreadSafe>>* DestinationQueue;
	FThreadSafeCounter StopTaskCounter=0;
	int32 GetIDFromPacket(TSharedPtr<TArray<uint8>> PacketData);

public:
	PacketDispatcher(TQueue<TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe>>* Queue, TQueue<TSharedPtr<TPair<int32, TArray<uint8>>, ESPMode::ThreadSafe>>* Dest);
	~PacketDispatcher() = default;

	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;
};