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
	UPROPERTY()
	class ULoadingScreenWidget* LoadingWidget;
	UPROPERTY()
	class UCreateCharacterUserWidget* CreateCharacterWidget;


protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	ALoginGameModeBase();

	virtual void ShowLoadingScreen() override;
	virtual void HideLoadingScreen() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UMyUserWidget> LoginWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class ULoadingScreenWidget> LoadingWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UCreateCharacterUserWidget> CreateCharacterWidgetClass;

	void SendHashAuthCheckPacket();
	void SendRequestCharBaseInfoPacket();

	virtual void OnLoginResponsePacketReceived(FLoginResponsePacket Packet) override;
	virtual void OnIDUnqiueCheckResponsePacketReceived(FIDUniqueCheckResponsePacket Packet) override;
	virtual void OnRegistAccountResponsePacketReceived(FRegistAccountResponsePacket Packet) override;
	virtual void OnHashAuthCheckResponsePacketReceived(FResponseHashAuthCheckPacket Packet) override;
	virtual void OnKickClientPacketReceived(FSendKickClientPacket Packet) override;
	virtual void OnResponseNeedToMakeCharacter(FResponseNeedToMakeCharcterPacket Packet) override;
	virtual void OnResponseCreateCharacter(FResponseCreateCharacterPacket Packet) override;
	virtual void OnResponseCreateNickName(FCreateNickNameResponsePacket Packet) override;
	virtual void OnResponseCharBaseInfo(FResponseCharBaseInfoPacket Packet) override;

	void ShowCreateCharacterWidget();
};
