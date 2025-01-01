// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TargetInterfaceWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKJCLIENT_API UTargetInterfaceWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NameTextBlock;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LevelTextBlock;

public :
	void SetTargetInfo(const APawn& Object);
private:
	void SetHPBarPercent(int CurrentHP, int MaxHP);
	void SetNameText(FString Name);
	void SetLevelText(int Level);
	void SetCharacterInfo(const class APlayerCharacter& Object);
	void SetNormalMonsterInfo(const class ANormalMonster& Object);
};
