// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateCharacterUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKJCLIENT_API UCreateCharacterUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* OKButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* GenderButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GenderTextBlock;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* NickNameEditTextBox;

	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundImage;

	UPROPERTY(meta = (BindWidget))
	class UListView* CharacterPresetListView;

	// 메타 휴먼 적용해보자
};
