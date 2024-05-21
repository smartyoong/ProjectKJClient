// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Networking.h"

/**
 * 
 */
class ClientSocket
{
private:
	FSocket* Socket;

	bool IsConnected = false;

public:
	ClientSocket();
	~ClientSocket();
	bool ConnectToLoginServer();
	void DisconnectFromServer();
	uint8* StartRecvFromServer();
	void SendToServer(const uint8* Data, int32 BytesToSend);
	FSocket* GetSocket() { return Socket; }
};
