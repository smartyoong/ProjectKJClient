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
	FSocket* LoginSocket;
	FSocket* GameSocket;
	bool IsConnected = false;

public:
	ClientSocket();
	~ClientSocket();
	bool ConnectToLoginServer();
	void DisconnectFromLoginServer();
	void StartRecvFromLoginServer();
	void SendToLoginServer(const uint8* Data, int32 BytesToSend);
	FSocket* GetLoginSocket() { return LoginSocket; }
	FSocket* GetGameSocket() { return GameSocket; }
};
