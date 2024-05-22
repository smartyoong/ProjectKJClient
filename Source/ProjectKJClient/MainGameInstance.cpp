// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"

void UMainGameInstance::Init()
{
	Super::Init();
	PacketQueue = new TQueue<TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe>>();
	LoginSockRun = new SocketThread(LOGIN_MODE,PacketQueue);
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
	Dispatcher = new PacketDispatcher(PacketQueue);
	DispatcherThread = FRunnableThread::Create(Dispatcher, TEXT("PacketDispatcherThread"));
}

void UMainGameInstance::Shutdown()
{
	Super::Shutdown();

	LoginThread->Kill(true);
	delete LoginThread;

	DispatcherThread->Kill(true);
	delete DispatcherThread;

	if (!PacketQueue->IsEmpty())
	{
		PacketQueue->Empty();
	}
	delete PacketQueue;
}

template<typename T>
void UMainGameInstance::SendPacketToLoginServer(LoginPacketListID ID, T Packet)
{
	LoginSockRun->SendPacket<T>(ID, Packet);
}