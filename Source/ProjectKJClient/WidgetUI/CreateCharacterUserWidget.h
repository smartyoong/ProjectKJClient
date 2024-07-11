// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UCreateCharacterPresetData.h"
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
	UPROPERTY()
	class ULoginResultWidget* ResultWidget;
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

	UPROPERTY(EditAnywhere, Category = "Entry")
	TArray<UMaterialInterface*> PresetImageMaterialList;

	UPROPERTY(EditAnywhere, Category = "Entry")
	TArray<FString> PresetNameList;

	UPROPERTY(EditDefaultsOnly, Category = "ResultUI")
	TSubclassOf<class ULoginResultWidget> ResultWidgetClass;

	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnListItemClick(UObject* Obj);
	UFUNCTION()
	void OnGenderButtonClick();
	UFUNCTION()
	void OnOKButtonClick();
	void PopulateList();

};
