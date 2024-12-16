// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKJCLIENT_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// HP MP 관련 변수들
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* MPProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HPTextBlock;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MPTextBlock;

public:
	UFUNCTION(BlueprintCallable, Category ="HPMP")
	void SetHP(int HP, int MaxHP);

	UFUNCTION(BlueprintCallable, Category = "HPMP")
	void SetMP(int MP, int MaxMP);

};
