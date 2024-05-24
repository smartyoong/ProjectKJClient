// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocketThread.h"
#include "JsonUtilities.h"

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

template <typename T>
void SocketThread::SendPacket(LoginPacketListID ID, T Packet)
{
	// 직렬화해서 Send 시키자 
	TArray<uint8> PacketData;
	
	PacketData.AddUninitialized(sizeof(int32));
	FMemory::Memcpy(PacketData.GetData(), &ID, sizeof(int32));
	FString JSonString;
	FJsonObjectConverter::UStructToJsonObjectString(Packet, JSonString);
	FTCHARToUTF8 Converter(*JSonString);
	TArray<uint8> data(Converter.Get(), Converter.Length());
	PacketData.Append(data);
	Socket.SendToServer(PacketData.GetData(), PacketData.Num());
	// 고정 패킷이면 아래도 괜찮을듯
	//TArray<uint8> Temp;
	//int32 Size = StringToBytes(JSonString, Temp.GetData(), Temp.Num());
}

