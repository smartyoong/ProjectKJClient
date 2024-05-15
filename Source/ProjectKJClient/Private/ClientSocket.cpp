// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"

ClientSocket::ClientSocket()
{
	LoginSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("LoginSocket"), false);
	GameSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("GameSocket"), false);
}

ClientSocket::~ClientSocket()
{
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(LoginSocket);
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(GameSocket);
}

bool ClientSocket::ConnectToLoginServer()
{
	LoginSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("LoginSocket"), false);
	FIPv4Address Addr;
	FString IP;
	int32 Port;
	// INI에서 IP를 읽어옵니다.
	if (!GConfig->GetString(TEXT("Network"), TEXT("LOGIN_IP"), IP, FPaths::ProjectConfigDir() / TEXT("Connect.ini")) or
		!GConfig->GetInt(TEXT("Network"), TEXT("LOGIN_PORT"), Port, FPaths::ProjectConfigDir() / TEXT("Connect.ini")))
	{
		return false;
	}
	// 주소 설정
	FIPv4Address::Parse(IP, Addr);
	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Addr.Value);
	InternetAddr->SetPort(Port);
	// 연결
	IsConnected = LoginSocket->Connect(*InternetAddr);
	if (!IsConnected)
	{
		// 추후 추가할것
		UE_LOG(LogTemp, Warning, TEXT("Failed to connect to Login server"));
		return false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Connected to Login server"));
		return true;
	}
}

void ClientSocket::DisconnectFromLoginServer()
{
	LoginSocket->Close();
}

void ClientSocket::StartRecvFromLoginServer()
{
	uint32 Size;
	while (LoginSocket->HasPendingData(Size))
	{
		int32 ReadSize = 0;
		uint8 Data[sizeof(int32)];
		LoginSocket->Recv(Data, sizeof(Data), ReadSize);
		if (ReadSize <= 0)
			break;
		int32 PacketSize = *(int32*)Data;
		if (PacketSize <= 0)
			break;
		uint8* PacketData = new uint8[PacketSize];
		LoginSocket->Recv(PacketData, PacketSize, ReadSize);
		if (ReadSize <= 0)
			break;
	}
}

void ClientSocket::SendToLoginServer(const uint8* Data, int32 BytesToSend)
{
	// 사이즈를 구해서 패킷에 추가로 붙이는거 구현해야함
	if (!LoginSocket->Send(Data, sizeof(Data), BytesToSend))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Send to Login server"));
	}
}
