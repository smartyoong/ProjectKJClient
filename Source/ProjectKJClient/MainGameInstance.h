// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Networking.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKJCLIENT_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	FSocket* LoginSocket;
	FSocket* GameSocket;
public:
	virtual void Init() override;
	virtual void Shutdown() override;
	FSocket* GetLoginSocket() { return LoginSocket; }
};
