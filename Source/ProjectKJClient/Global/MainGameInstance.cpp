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
	LoginDispatcherThread = FRunnableThread::Create(LoginDispatcher, TEXT("LoginPacketDispatcherThread"));

	LoginPacketProcessor = new PacketProcessor(LoginDestinationPacketQueue, PacketProcessorMode::LOGIN);
	LoginPacketProcessorThread = FRunnableThread::Create(LoginPacketProcessor, TEXT("LoginPacketProcessorThread"));


	///////////////////////////////////// 게임 서버와 연결 /////////////////////////////////////
	GamePacketQueue = new TQueue<TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe>>();
	GameDestinationPacketQueue = new TQueue<TSharedPtr<TPair<int32, TArray<uint8>>, ESPMode::ThreadSafe>>();
	GameSockRun = new SocketThread(GAME_MODE,GamePacketQueue);
	GameThread = FRunnableThread::Create(GameSockRun, TEXT("GameSocketThread"));
	if (!GameSockRun -> GetInitSuccess())
	{
		FText Message = FText::FromString(TEXT("게임 서버와 연결을 실패하였습니다.\n서버 점검중인지 홈페이지를 확인해주세요"));
		FText Title = FText::FromString(TEXT("연결 실패"));
		// 메시지 박스 출력
		FMessageDialog::Open(EAppMsgType::Ok,Message,Title);
		// 게임 종료
		FGenericPlatformMisc::RequestExit(false);
	}
	GameDispatcher = new PacketDispatcher(GamePacketQueue,GameDestinationPacketQueue);
	GameDispatcherThread = FRunnableThread::Create(GameDispatcher, TEXT("GamePacketDispatcherThread"));

	GamePacketProcessor = new PacketProcessor(GameDestinationPacketQueue, PacketProcessorMode::INGAME);
	GamePacketProcessorThread = FRunnableThread::Create(GamePacketProcessor, TEXT("GamePacketProcessorThread"));

	Loader = MakeShareable(new ResourceLoader());
	if (!Loader->PrepareToLoad())
	{
		FText Message = FText::FromString(TEXT("리소스 폴더를 찾을 수 없습니다.\n리소스 폴더를 확인해주세요"));
		FText Title = FText::FromString(TEXT("리소스 폴더 없음"));
		// 메시지 박스 출력
		FMessageDialog::Open(EAppMsgType::Ok,Message,Title);
		// 게임 종료
		FGenericPlatformMisc::RequestExit(false);
	}
	LoadResource();
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

	/////////////////////////////////////////////////


	GameThread->Kill(true);
	delete GameThread;

	GameDispatcherThread->Kill(true);
	delete GameDispatcherThread;

	GamePacketProcessor->Stop();
	GamePacketProcessorThread->Kill(false);
	delete GamePacketProcessorThread;

	if (!GamePacketQueue->IsEmpty())
	{
		GamePacketQueue->Empty();
	}
	delete GamePacketQueue;

	if (!GameDestinationPacketQueue->IsEmpty())
	{
		GameDestinationPacketQueue->Empty();
	}
	delete GameDestinationPacketQueue;
}

void UMainGameInstance::RegistGameModeToPacketQueue(ACommonGameModeBase* GameMode)
{
	LoginPacketProcessor->SetGameMode(GameMode);
	GamePacketProcessor->SetGameMode(GameMode);
}

void UMainGameInstance::LoadResource()
{
	Loader->LoadChracterPresetInfo(ResChracterPresetMap);
	Loader->LoadMapInfo(ResMapInfoMap);
}

FString UMainGameInstance::GetMapNameByMapID(int32 MapID)
{
	if (ResMapInfoMap.Contains(MapID))
	{
		return ResMapInfoMap[MapID].MapName;
	}
	return FString();
}

void UMainGameInstance::SetCharacterPresetID(int32 ID)
{
	FScopeLock Lock(&CriticalSection);
	CharacterSpawnInfo.CharacterPresetID = ID;
}

void UMainGameInstance::SetFirstSpawnLocation(FVector Location)
{
	FScopeLock Lock(&CriticalSection);
	CharacterSpawnInfo.FirstSpawnLocation = Location;
}

void UMainGameInstance::SetSpawnMapID(int32 ID)
{
	FScopeLock Lock(&CriticalSection);
	CharacterSpawnInfo.SpawnMapID = ID;
}

void UMainGameInstance::SetCharacterGender(int32 Gender)
{
	FScopeLock Lock(&CriticalSection);
	CharacterSpawnInfo.Gender;
}

void UMainGameInstance::SetCharacterJob(int32 Job)
{
	FScopeLock Lock(&CriticalSection);
	CharacterSpawnInfo.Job = Job;
}

void UMainGameInstance::SetCharacterJobLevel(int32 JobLevel)
{
	FScopeLock Lock(&CriticalSection);
	CharacterSpawnInfo.JobLevel = JobLevel;
}

void UMainGameInstance::SetCharacterLevel(int32 Level)
{
	FScopeLock Lock(&CriticalSection);
	CharacterSpawnInfo.Level = Level;
}

void UMainGameInstance::SetCharacterEXP(int32 EXP)
{
	FScopeLock Lock(&CriticalSection);
	CharacterSpawnInfo.EXP = EXP;
}

void UMainGameInstance::SetCharacterHPMP(int32 HP, int32 MP)
{
	FScopeLock Lock(&CriticalSection);
	CharacterSpawnInfo.HP = HP;
	CharacterSpawnInfo.MP = MP;
}

FString UMainGameInstance::GetCharacterPresetBPPath(int32 ID)
{
	if (ResChracterPresetMap.Contains(ID))
	{
		return ResChracterPresetMap[ID].PlayerCharacterName;
	}
	return FString();
}

void UMainGameInstance::MakeKoreanToUTF8(const FString& OriginString, FString& DestUTF8String)
{
	FTCHARToUTF8 Convert(*OriginString);
	DestUTF8String = FString(Convert.Length(), Convert.Get());
}
