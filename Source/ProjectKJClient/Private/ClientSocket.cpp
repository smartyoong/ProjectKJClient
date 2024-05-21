// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"

ClientSocket::ClientSocket()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("ClientSocket"), false);
}

ClientSocket::~ClientSocket()
{
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
}

bool ClientSocket::ConnectToLoginServer()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("ClientSocket"), false);
	FIPv4Address Addr;
	FString IP;
	int32 Port = 0;
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
	IsConnected = Socket->Connect(*InternetAddr);
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

void ClientSocket::DisconnectFromServer()
{
	Socket->Close();
}

uint8* ClientSocket::StartRecvFromServer()
{
	uint32 Size;
	while (Socket->HasPendingData(Size))
	{
		// 사이즈를 구해서 해당 사이즈만큼 데이터를 받는다.
		int32 ReadSize = 0;
		uint8 Data[sizeof(int32)];
		Socket->Recv(Data, sizeof(Data), ReadSize);
		if (ReadSize <= 0)
			break;
		int32 PacketSize = 0;
		FMemory::Memcpy(&PacketSize, Data, sizeof(int32));
		if (PacketSize <= 0)
			break;
		uint8* PacketData = new uint8[PacketSize];
		Socket->Recv(PacketData, PacketSize, ReadSize);
		if (ReadSize <= 0)
		{
			delete[] PacketData;
			break;
		}
		return PacketData;
		// 반드시 Delete 시킬 것
	}
	return nullptr;
}

void ClientSocket::SendToServer(const uint8* Data, int32 BytesToSend)
{
	// 패킷 사이즈를 앞에 붙여서 보냅니다.
	int32 SendSize = 0;

	TArray<uint8> PacketArray;
	PacketArray.AddUninitialized(sizeof(int32));
	FMemory::Memcpy(PacketArray.GetData(), &BytesToSend, sizeof(int32));
	PacketArray.Append(Data, BytesToSend);

	if (!Socket->Send(PacketArray.GetData(), PacketArray.Num(), SendSize))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Send to Login server"));
	}
	else if (SendSize <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Send to Login server2"));
	}
}
