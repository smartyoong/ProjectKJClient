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

	LoginPacketProcessor = new PacketProcessor(LoginDestinationPacketQueue, PacketProcessorMode::LOGIN);
	LoginPacketProcessorThread = FRunnableThread::Create(LoginPacketProcessor, TEXT("PacketProcessorThread"));

}

void UMainGameInstance::Shutdown()
{
	Super::Shutdown();

	LoginThread->Kill(true);
	delete LoginThread;

	LoginDispatcherThread->Kill(true);
	delete LoginDispatcherThread;

	LoginPacketProcessor->Stop();
	LoginPacketProcessorThread->Kill(false);
	delete LoginPacketProcessorThread;

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

void UMainGameInstance::RegistGameModeToPacketQueue(ACommonGameModeBase* GameMode)
{
	LoginPacketProcessor->SetGameMode(GameMode);
}