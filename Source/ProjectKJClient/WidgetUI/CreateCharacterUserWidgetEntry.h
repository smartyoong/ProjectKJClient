// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UCreateCharacterPresetData.h"
#include "CreateCharacterUserWidgetEntry.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKJCLIENT_API UCreateCharacterUserWidgetEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* PresetImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NameTextBlock;

	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundImage;

	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* Obj) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

	void UpdateEntry(UTexture2D* ImageData, FString TextData);
};
