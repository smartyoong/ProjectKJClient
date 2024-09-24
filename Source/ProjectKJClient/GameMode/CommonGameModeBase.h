// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LoginPacketList.h"
#include "GamePacketList.h"
#include "CommonGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ACommonGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
private:
	void CloseGameWithErrorMsgBox()
	{

		// 메시지 박스 표시
		FText Title = FText::FromString(TEXT("가상 메서드를 구현하십쇼"));
		FText Content = FText::FromString(TEXT("ACommonGameModeBase에 메서드 중에서\n CloseGameWithErrorMsgBox를 호출하는 메서드들을\n 반드시 전부 재정의 하십시오."));
		EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::OkCancel, Content, Title);

		// 사용자의 응답에 따라 처리
		if (ReturnType == EAppReturnType::Ok)
		{
			// 프로그램 종료
			FGenericPlatformMisc::RequestExit(false);
		}
		else
		{
			// 프로그램 종료
			FGenericPlatformMisc::RequestExit(false);
		}
	}

public:
	virtual ~ACommonGameModeBase() = default;

	// 앞으로 로직 함수를 여기서 선언하자
	virtual void ShowLoadingScreen()
	{
		CloseGameWithErrorMsgBox();
	};
	virtual void HideLoadingScreen() 
	{
		CloseGameWithErrorMsgBox();
	};
	// 해시 인증 응답 함수  ---> 이건 모든 게임 모드에서 구현하도록 해야함
	virtual void OnHashAuthCheckResponsePacketReceived(FResponseHashAuthCheckPacket Packet)
	{
		CloseGameWithErrorMsgBox();
	};

	// 로그인 응답 함수
	virtual void OnLoginResponsePacketReceived(FLoginResponsePacket Packet) {};
	// ID 중복 체크 응답 함수
	virtual void OnIDUnqiueCheckResponsePacketReceived(FIDUniqueCheckResponsePacket Packet) {};
	// 회원 가입 응답 함수
	virtual void OnRegistAccountResponsePacketReceived(FRegistAccountResponsePacket) {};
	// 강제 추방 함수
	virtual void OnKickClientPacketReceived(FSendKickClientPacket Packet) {};
	// 새로 캐릭터를 만들어야함
	virtual void OnResponseNeedToMakeCharacter(FResponseNeedToMakeCharcterPacket Packet) {};
	// 캐릭터 생성	응답 함수
	virtual void OnResponseCreateCharacter(FResponseCreateCharacterPacket Packet) {};
	// 닉네임 생성 응답 함수
	virtual void OnResponseCreateNickName(FCreateNickNameResponsePacket Packet) {};
	// 캐릭터 정보 요청 응답 함수
	virtual void OnResponseCharBaseInfo(FResponseCharBaseInfoPacket Packet) {};
	// 캐릭터 이동 응답 함수
	virtual void OnResponseMoveCharacter(FResponseMovePacket Packet) {};
	// 같은 맵의 다른 캐릭터를 그리도록 명령하는 함수
	virtual void OnSendAnotherCharBaseInfo(FSendAnotherCharBaseInfoPacket Packet) {};
	// 같은 맵의 다른 유저를 이동 시키는 함수
	virtual void OnSendUserMove(FSendUserMovePacket Packet) {};
	// 핑 체크하는 함수
	virtual void OnResponsePingCheck(FResponsePingCheckPacket Packet) {};
	// 유저 도착시 다른 맵 유저들에게 최종위치로 이동시키도록 전달하는 함수
	virtual void OnSendUserMoveArrived(FSendUserMoveArrivedPacket Packet) {};
};
