// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CommonGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ACommonGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual ~ACommonGameModeBase() = default;
	// 인스턴스에 현재 사용할 게임모드를 설정합니다. 이는 패킷을 받아오기에 매우 중요합니다.
	virtual void RegistGameMode() {};
	// 패킷을 받아오는 함수입니다.
	virtual void GetPacketFromQueue() {};
};
