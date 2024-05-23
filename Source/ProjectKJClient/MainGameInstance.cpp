// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"

void UMainGameInstance::Init()
{
	Super::Init();
	LoginPacketQueue = new TQueue<TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe>>();
	LoginDestinationPacketQueue = new TQueue<TSharedPtr<TPair<int32, TArray<uint8>>, ESPMode::ThreadSafe>>();
	LoginSockRun = new SocketThread(LOGIN_MODE,LoginPacketQueue);
	LoginThread = FRunnableThread::Create(LoginSockRun, TEXT("LoginSocketThread"));
	if (!LoginSockRun -> GetInitSuccess())
	{
		FText Message = FText::FromString(TEXT("로그인 서버와 연결을 실패하였습니다.\n서버 점검중인지 홈페이지를 확인해주세요"));
		FText Title = FText::FromString(TEXT("연결 실패"));
		// 메시지 박스 출력
		FMessageDialog::Open(EAppMsgType::Ok,Message,Title);
		// 게임 종료
		FGenericPlatformMisc::RequestExit(false);
	}
	LoginDispatcher = new PacketDispatcher(LoginPacketQueue,LoginDestinationPacketQueue);
	LoginDispatcherThread = FRunnableThread::Create(LoginDispatcher, TEXT("PacketDispatcherThread"));
}

void UMainGameInstance::Shutdown()
{
	Super::Shutdown();

	LoginThread->Kill(true);
	delete LoginThread;

	LoginDispatcherThread->Kill(true);
	delete LoginDispatcherThread;

	if (!LoginPacketQueue->IsEmpty())
	{
		LoginPacketQueue->Empty();
	}
	delete LoginPacketQueue;

	if (!LoginDestinationPacketQueue->IsEmpty())
	{
		LoginDestinationPacketQueue->Empty();
	}
	delete LoginDestinationPacketQueue;
}

template<typename T>
void UMainGameInstance::SendPacketToLoginServer(LoginPacketListID ID, T Packet)
{
	LoginSockRun->SendPacket<T>(ID, Packet);
    // 추후 아래 코드로 전송시 직렬화 생각해볼것 JsonString -> uint8
	//int32 Utf8Length = FTCHARToUTF8_Convert::ConvertedLength(*ContentString, ContentString.Len());
	//TArray<uint8> Buffer;
	//Buffer.SetNumUninitialized(Utf8Length);
	//FTCHARToUTF8_Convert::Convert((UTF8CHAR*)Buffer.GetData(), Buffer.Num(), *ContentString, ContentString.Len());
	//int32 StringToBytes
	//(
	//	const FString & String,
	//	uint8 * OutBytes,
	//	int32 MaxBufferSize
	//) UnrealString.h
}