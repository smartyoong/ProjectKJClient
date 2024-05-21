// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocketThread.h"

SocketThread::SocketThread(int mode, TQueue<uint8*>* InPacketQueue) : Mode(mode), PacketQueue(InPacketQueue)
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
}

