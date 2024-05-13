// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"

void UMainGameInstance::Init()
{
	Super::Init();
	LoginSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("MainSocket"), false);
	FIPv4Address Addr;
	FIPv4Address::Parse("192.168.219.102",Addr);
	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Addr.Value);
	InternetAddr->SetPort(12475);
	bool IsConnected = LoginSocket->Connect(*InternetAddr);
	if (!IsConnected)
	{
		// 추후 추가할것
		UE_LOG(LogTemp, Warning, TEXT("Failed to connect to Login server"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Connected to Login server"));
	}
}

void UMainGameInstance::Shutdown()
{
	Super::Shutdown();
	LoginSocket->Close();
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(LoginSocket);
}