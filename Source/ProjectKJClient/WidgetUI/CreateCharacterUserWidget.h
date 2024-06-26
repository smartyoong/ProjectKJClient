// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GlobalVariable.h"
#include "CreateCharacterUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKJCLIENT_API UCreateCharacterUserWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	bool IsKoreanOrAlphaNumeric(TCHAR Char);
	bool IsNickNameValid(const FString& InputString);
	bool IsMale = true;
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
	class UImage* CharacterImage;

	UPROPERTY(meta = (BindWidget))
	class UListView* CharacterPresetListView;

	TArray<UCreateCharacterPresetData*> PresetDataList;

	UPROPERTY(EditAnywhere, Category = "Entry")
	TArray<UMaterialInterface*> PresetImageMaterialList;

	UPROPERTY(EditAnywhere, Category = "Entry")
	TArray<FString> PresetNameList;

	virtual void NativeOnInitialized() override;
	void OnListItemClick(UObject* Obj);
	void OnGenderButtonClick();
	void OnOKButtonClick();
};
