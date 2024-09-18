// Fill out your copyright notice in the Description page of Project Settings.


#include "PacketProcessor.h"

PacketProcessor::PacketProcessor(TQueue<TSharedPtr<TPair<int32, TArray<uint8>>, ESPMode::ThreadSafe>>* Queue, 
	PacketProcessorMode mode) : PacketQueue(Queue), ProcessMode(mode), StopTaskCounter(0)
{
}

PacketProcessor::~PacketProcessor()
{
}

bool PacketProcessor::Init()
{
	if(PacketQueue == nullptr)
		return false;
	GameModeEvent = FPlatformProcess::GetSynchEventFromPool();
	return true;
}

uint32 PacketProcessor::Run()
{
	GameModeEvent->Wait();

	while(!StopTaskCounter.GetValue())
	{

		TSharedPtr<TPair<int32, TArray<uint8>>> PacketData;
		if (PacketQueue != nullptr && PacketQueue->Dequeue(PacketData))
		{
			if (!PacketData.IsValid())
				continue;
			// 패킷 처리
			switch (ProcessMode)
			{
				case PacketProcessorMode::LOGIN:
					ProcessLoginPacket(PacketData);
					break;
				case PacketProcessorMode::INGAME:
					ProcessGamePacket(PacketData);
					break;
				default:
					UE_LOG(LogTemp, Error, TEXT("Invalid PacketProcessor Mode"));
					break;
			}
		}
	}
	return 0;
}

void PacketProcessor::Stop()
{
	StopTaskCounter.Increment();
	GameModeEvent->Trigger();
}

void PacketProcessor::Exit()
{
	FPlatformProcess::ReturnSynchEventToPool(GameModeEvent);
}

void PacketProcessor::ProcessLoginPacket(TSharedPtr<TPair<int32, TArray<uint8>>> PacketData)
{
	LoginPacketListID ID = static_cast<LoginPacketListID>(PacketData->Key);
	switch (ID)
	{
		case LoginPacketListID::LOGIN_REQUEST:
			break;
		case LoginPacketListID::LOGIN_RESPONESE:
			GameMode->OnLoginResponsePacketReceived(PacketToStruct<FLoginResponsePacket>(PacketData->Value));
			break;
		case LoginPacketListID::REGIST_ACCOUNT_REQUEST:
			break;
		case LoginPacketListID::REGIST_ACCOUNT_RESPONESE:
			GameMode->OnRegistAccountResponsePacketReceived(PacketToStruct<FRegistAccountResponsePacket>(PacketData->Value));
			break;
		case LoginPacketListID::ID_UNIQUE_CHECK_REQUEST:
			break;
		case LoginPacketListID::ID_UNIQUE_CHECK_RESPONESE:
			GameMode->OnIDUnqiueCheckResponsePacketReceived(PacketToStruct<FIDUniqueCheckResponsePacket>(PacketData->Value));
			break;
		case LoginPacketListID::CREATE_NICKNAME_RESPONESE:
			GameMode->OnResponseCreateNickName(PacketToStruct<FCreateNickNameResponsePacket>(PacketData->Value));
			break;
		default:
			break;
			// 패킷을 진짜로 처리하고 GameModeBase를 장착시키고 인스턴스에서 이 스레드를 실행시키자
	}
}


void PacketProcessor::ProcessGamePacket(TSharedPtr<TPair<int32, TArray<uint8>>> PacketData)
{
	GamePacketListID ID = static_cast<GamePacketListID>(PacketData->Key);
	//UE_LOG(LogTemp, Warning, TEXT("GamePacketListID : %d"), ID);
	switch (ID)
	{
	case GamePacketListID::REQUEST_HASH_AUTH_CHECK:
		break;
	case GamePacketListID::RESPONSE_HASH_AUTH_CHECK:
		//UE_LOG(LogTemp, Warning, TEXT("RESPONSE_HASH_AUTH_CHECK"));
		GameMode->OnHashAuthCheckResponsePacketReceived(PacketToStruct<FResponseHashAuthCheckPacket>(PacketData->Value));
		break;
	case GamePacketListID::KICK_CLIENT:
		GameMode->OnKickClientPacketReceived(PacketToStruct<FSendKickClientPacket>(PacketData->Value));
		break;
	case GamePacketListID::RESPONSE_NEED_TO_MAKE_CHARACTER:
		GameMode->OnResponseNeedToMakeCharacter(PacketToStruct<FResponseNeedToMakeCharcterPacket>(PacketData->Value));
		break;
	case GamePacketListID::RESPONSE_CREATE_CHARACTER:
		GameMode->OnResponseCreateCharacter(PacketToStruct<FResponseCreateCharacterPacket>(PacketData->Value));
		break;
	case GamePacketListID::RESPONSE_CHAR_BASE_INFO:
		GameMode->OnResponseCharBaseInfo(PacketToStruct<FResponseCharBaseInfoPacket>(PacketData->Value));
		break;
	case GamePacketListID::RESPONSE_MOVE:
		GameMode->OnResponseMoveCharacter(PacketToStruct<FResponseMovePacket>(PacketData->Value));
		break;
	case GamePacketListID::SEND_ANOTHER_CHAR_BASE_INFO:
		GameMode->OnSendAnotherCharBaseInfo(PacketToStruct<FSendAnotherCharBaseInfoPacket>(PacketData->Value));
		break;
	default:
		break;
	}

}

void PacketProcessor::SetGameMode(ACommonGameModeBase* Mode)
{
	FCriticalSection Lock;
	Lock.Lock();
	this->GameMode = Mode;
	GameModeEvent->Trigger();
	Lock.Unlock();
}