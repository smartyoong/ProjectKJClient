// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginResultWidget.h"
#include "RegistAccountUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKJCLIENT_API URegistAccountUserWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	FString ID;
	FString Password;
	FString PasswordCheck;
	bool IsIDUnique = false;
	class UMyUserWidget* ParentWidget;
	UPROPERTY()
	class ULoginResultWidget* RegistAccountResultWidget;
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnIDUniqueCheckButtonClicked();

	UFUNCTION()
	void OnRegistButtonClicked();

	UFUNCTION()
	void OnIDTextCommitted(const FText& text, ETextCommit::Type TextType);

	UFUNCTION()
	void OnIDTextChange(const FText& text);

	UFUNCTION()
	void OnPasswordTextCommitted(const FText& text, ETextCommit::Type TextType);

	UFUNCTION()
	void OnPasswordCheckTextCommitted(const FText& text, ETextCommit::Type TextType);

	UFUNCTION()
	void OnCancelButtonClicked();

	UFUNCTION()
	void OnPasswordChanged(const FText& text);

	UFUNCTION()
	void OnPasswordCheckChanged(const FText& text);
public :

	UPROPERTY(EditDefaultsOnly, Category = "ResultWidget")
	TSubclassOf<class ULoginResultWidget> RegistAccountResultWidgetClass;

	UPROPERTY(meta = (BindWidget))
	class UButton* IDButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* RegistButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* IDTextBox;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* PWTextBox;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* PWCheckTextBox;

	void SetParentWidget(UMyUserWidget* Parent) { ParentWidget = Parent; }

	void ShowIDIsNotUnique();
	void ShowIDIsUnique();
	void ShowRegistSuccess();
	void ShowRegistFail(int Mode);
};
