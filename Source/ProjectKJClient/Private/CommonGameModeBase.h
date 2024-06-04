// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LoginPacketManager.h"
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
	// 앞으로 로직 함수를 여기서 선언하자

	// 로그인 응답 함수
	virtual void OnLoginResponsePacketReceived(FLoginResponsePacket Packet) {};
	// ID 중복 체크 응답 함수
	virtual void OnIDUnqiueCheckResponsePacketReceived(FIDUniqueCheckResponsePacket Packet) {};
	// 회원 가입 응답 함수
	virtual void OnRegistAccountResponsePacketReceived(FRegistAccountResponsePacket) {};

};
