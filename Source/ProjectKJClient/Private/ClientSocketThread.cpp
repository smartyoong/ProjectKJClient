// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocketThread.h"

SocketThread::SocketThread(int mode, TQueue<TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe>>* InPacketQueue) : Mode(mode), PacketQueue(InPacketQueue)
{
}

bool SocketThread::Init()
{
	if (PacketQueue == nullptr)
	{
		InitSuccess = false;
		return false;
	}

	if (Mode == LOGIN_MODE)
	{
		if (!Socket.ConnectToLoginServer())
		{
			InitSuccess = false;
			return true;
		}
		else
		{
			InitSuccess = true;
			return false;
		}
	}
	else
	{
		// 임시
		return false;
	}
}

uint32 SocketThread::Run()
{
    while (!StopTaskCounter.GetValue())
    {
		auto PacketData = Socket.StartRecvFromServer();
		if (PacketQueue != nullptr)
		{
			if (!PacketData.IsValid())
				continue;

			PacketQueue->Enqueue(PacketData);
		}
    }
    return 0;
}

void SocketThread::Stop()
{
	StopTaskCounter.Increment();
}

void SocketThread::Exit()
{
    Socket.DisconnectFromServer();
}

