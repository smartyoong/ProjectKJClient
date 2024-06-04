// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CommonGameModeBase.h"
#include "LoginGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKJCLIENT_API ALoginGameModeBase : public ACommonGameModeBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class UMyUserWidget* LoginWidget;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	ALoginGameModeBase();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UMyUserWidget> LoginWidgetClass;

	virtual void OnLoginResponsePacketReceived(FLoginResponsePacket Packet) override;
	virtual void OnIDUnqiueCheckResponsePacketReceived(FIDUniqueCheckResponsePacket Packet) override;
	virtual void OnRegistAccountResponsePacketReceived(FRegistAccountResponsePacket Packet) override;
};
