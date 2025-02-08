// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonGameModeBase.h"
#include "WorldInfoToJson.h"
#include "StartMapGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKJCLIENT_API AStartMapGameModeBase : public ACommonGameModeBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class ULoadingScreenWidget* LoadingWidget;
	UPROPERTY()
	class APlayerCharacter* PlayerCharacter;
	UPROPERTY()
	TSubclassOf<class APlayerCharacter> PlayerCharacterClass;
	TArray<class APlayerCharacter*> AnotherPlayerCharacterList;
	FCriticalSection CriticalSection;

public:
	AStartMapGameModeBase();
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class ULoadingScreenWidget> LoadingWidgetClass;
	void ShowLoadingScreen();
	void HideLoadingScreen();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void SpawnPlayerCharacter(FVector Position, FRotator Rotation);
	virtual void OnResponseMoveCharacter(FResponseMovePacket Packet) override;
	virtual void OnSendAnotherCharBaseInfo(FSendAnotherCharBaseInfoPacket Packet) override;
	void GetSameMapUser(class APlayerCharacter* Player);
	virtual void OnSendUserMove(FSendUserMovePacket Packet);
	virtual void OnSendUserMoveArrived(FSendUserMoveArrivedPacket Packet);
	virtual void OnResponsePingCheck(FResponsePingCheckPacket Packet);
	virtual void OnChatPacketReceived(FSendUserSayPacket Packet);
};
