// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMapGameModeBase.h"
#include "WorldInfoToJson.h"
#include "ProjectKJClient/MainGameInstance.h"

void AStartMapGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetGameInstance<UMainGameInstance>()->RegistGameModeToPacketQueue(this);

	if (GetWorld() != nullptr)
	{
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
	}

	
	// 맵 정보 읽어올때 사용
	//WorldInfoToJson::SaveWorldInfoToJson(GetWorld(), 0);
	//WorldInfoToJson::SaveWorldPortalInfoToJson(GetWorld(), 0);
}

void AStartMapGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
