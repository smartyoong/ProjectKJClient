// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMapGameModeBase.h"
#include "WorldInfoToJson.h"
#include "MainGameInstance.h"
#include "LoadingScreenWidget.h"
#include "PlayerCharacter.h"



//이제 여기 생성자에서 메인 게임 인스턴스를 통해 캐릭터 BP를 로드하고, BeginPlay에서 소환하자

AStartMapGameModeBase::AStartMapGameModeBase()
{
	//PlayerControllerClass = APlayerController::StaticClass();
	//static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Controller/BP_HumanPlayerController"));
	//if (PlayerControllerBPClass.Class != NULL)
	//{
	//	PlayerControllerClass = PlayerControllerBPClass.Class;
	//}
}

void AStartMapGameModeBase::ShowLoadingScreen()
{
	if (LoadingWidget != nullptr)
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				LoadingWidget->AddToViewport();
			});
	}

}

void AStartMapGameModeBase::HideLoadingScreen()
{
	if (LoadingWidget != nullptr)
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				LoadingWidget->RemoveFromParent();
			});
	}

}

void AStartMapGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetGameInstance<UMainGameInstance>()->RegistGameModeToPacketQueue(this);
	if (GetWorld() != nullptr)
	{
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
	}
	LoadingWidget = CreateWidget<ULoadingScreenWidget>(GetWorld(), LoadingWidgetClass);
	// 맵 정보 읽어올때 사용
	//WorldInfoToJson::SaveWorldInfoToJson(GetWorld(), 0);
	//WorldInfoToJson::SaveWorldPortalInfoToJson(GetWorld(), 0);

	// 캐릭터 소환 시작
	FCharacterInfo Info = Cast<UMainGameInstance>(GetWorld()->GetGameInstance())->GetCharacterSpawnInfo();
	//BP를 읽어와서 Info를 기반으로 실제 소환하자
	int32 BP_ID = Info.CharacterPresetID;
	FString BP_Name = Cast<UMainGameInstance>(GetWorld()->GetGameInstance())->GetCharacterPresetBPPath(BP_ID);
	PlayerCharacterClass = LoadClass<ACharacter>(nullptr, *BP_Name);
	SpawnPlayerCharacter(Info.FirstSpawnLocation, FRotator(0, 0, 0));
	// 다른 플레이어 캐릭터 소환
	GetSameMapUser(PlayerCharacter);
}

void AStartMapGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// 정리
	for (auto a : AnotherPlayerCharacterList)
	{
		a->Destroy();
	}
}

void AStartMapGameModeBase::SpawnPlayerCharacter(FVector Position, FRotator Rotation)
{
	if (GetWorld() == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GetWorld() == nullptr StartMapGameMode"));
		return;
	}

	if (PlayerCharacterClass != nullptr)
	{
		PlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(PlayerCharacterClass, Position, Rotation);
		if (PlayerCharacter != nullptr)
		{
			if (GetWorld()->GetFirstPlayerController() != nullptr)
			{
				GetWorld()->GetFirstPlayerController()->Possess(PlayerCharacter);
				PlayerCharacter->SetSpawnBaseInfo(Cast<UMainGameInstance>(GetWorld()->GetGameInstance())->GetCharacterSpawnInfo());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("플레이어 컨트롤러 BP를 찾을 수 없습니다. StartMapGameMode"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("플레이어 캐릭터를 소환할 수 없습니다. StartMapGameMode"));
		}
	}
}

void AStartMapGameModeBase::OnResponseMoveCharacter(FResponseMovePacket Packet)
{
	const int32 FAIL = 1;
	if (PlayerCharacter != nullptr)
	{
		if (Packet.ErrorCode == FAIL)
		{
			if (!IsInGameThread())
			{
				// 게임 스레드에서 실행되도록 큐에 추가
				AsyncTask(ENamedThreads::GameThread, [this]()
					{
						PlayerCharacter->RollBackLocation();
					});
				return;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnResponseMoveCharacter 플레이어 캐릭터가 없습니다. StartMapGameMode"));
	}
}

void AStartMapGameModeBase::OnSendAnotherCharBaseInfo(FSendAnotherCharBaseInfoPacket Packet)
{
	UE_LOG(LogTemp, Warning, TEXT("다른 플레이어 캐릭터 소환"));
	AsyncTask(ENamedThreads::GameThread, [this, Packet]()
		{
			int32 BP_ID = Packet.PresetNumber;
			FString BP_Name = Cast<UMainGameInstance>(GetWorld()->GetGameInstance())->GetCharacterPresetBPPath(BP_ID);
			auto AnotherClass = LoadClass<ACharacter>(nullptr, *BP_Name);
			if (GetWorld() == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("GetWorld() == nullptr StartMapGameMode"));
				return;
			}
			if (AnotherClass != nullptr)
			{
				auto AnotherPlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(AnotherClass, FVector(Packet.X, Packet.Y, 0), FRotator(0, 0, 0));
				if (AnotherPlayerCharacter != nullptr)
				{
					FCharacterInfo CharacterSpawnInfo;
					CharacterSpawnInfo.AccountID = Packet.AccountID;
					CharacterSpawnInfo.CharacterPresetID = Packet.PresetNumber;
					CharacterSpawnInfo.EXP = Packet.EXP;
					CharacterSpawnInfo.FirstSpawnLocation = FVector(Packet.X, Packet.Y, 0);
					CharacterSpawnInfo.Gender = Packet.Gender;
					CharacterSpawnInfo.Job = Packet.Job;
					CharacterSpawnInfo.JobLevel = Packet.JobLevel;
					CharacterSpawnInfo.Level = Packet.Level;
					CharacterSpawnInfo.NickName = Packet.NickName;
					CharacterSpawnInfo.SpawnMapID = Packet.MapID;
					AnotherPlayerCharacter->SetSpawnBaseInfo(CharacterSpawnInfo);
					// 주의 해당 캐릭터는 가비지 컬렉션이 작동하지 않습니다.
					FScopeLock Lock(&CriticalSection);
					AnotherPlayerCharacterList.Add(AnotherPlayerCharacter);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("다른 플레이어 캐릭터를 소환할 수 없습니다. StartMapGameMode"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("다른 플레이어 캐릭터 BP를 찾을 수 없습니다. StartMapGameMode"));
			}
		});
}

void AStartMapGameModeBase::GetSameMapUser(APlayerCharacter* Player)
{
	if (Player == nullptr)
	{
		return;
	}
	// 현재 맵에 있는 다른 플레이어 캐릭터 정보를 요청
	FRequestGetSameMapUserPacket Packet;
	Packet.AccountID = Cast<UMainGameInstance>(GetWorld()->GetGameInstance())->GetAccountID();
	Packet.HashCode = Cast<UMainGameInstance>(GetWorld()->GetGameInstance())->GetUserAuthHashCode();
	Packet.MapID = Player->GetMapID();
	Cast<UMainGameInstance>(GetWorld()->GetGameInstance())->SendPacketToGameServer(GamePacketListID::REQUEST_GET_SAME_MAP_USER, Packet);	
}

void AStartMapGameModeBase::OnSendUserMove(FSendUserMovePacket Packet)
{
	FScopeLock Lock(&CriticalSection);
	for (auto p : AnotherPlayerCharacterList)
	{
		if (p->GetAccountID() == Packet.AccountID)
		{
			if (!IsInGameThread())
			{
				// 게임 스레드에서 실행되도록 큐에 추가
				AsyncTask(ENamedThreads::GameThread, [this,Packet,p]()
					{
						p->MoveToLocation(FVector(Packet.X, Packet.Y, p->GetActorLocation().Z));
					});
			}
			return;
		}
	}
}

void AStartMapGameModeBase::OnResponsePingCheck(FResponsePingCheckPacket Packet)
{
	FDateTime Now = FDateTime::Now();
	FDateTime Start (Now.GetYear(), Now.GetMonth(), Now.GetDay(), Packet.Hour, Packet.Min, Packet.Secs, Packet.MSecs); // 밀리초를 마이크로초로 변환
	FTimespan Span = Now - Start;
	FString Message = FString::Printf(TEXT("Ping : %.2f ms"), Span.GetTotalMilliseconds());
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Message);
	}
}
