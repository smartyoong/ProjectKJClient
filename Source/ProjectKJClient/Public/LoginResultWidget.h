// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKJCLIENT_API ULoginResultWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnOkButtonClicked();
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* OKButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ResultTextBlock;

	void SetLoginSuceess();
	void SetNoAccount();
	void SetPasswordFail();
	void SetIDIsEmpty();
	void SetIDUniqueCheckFail();
	void SetIDUniqueCheckSuccess();
	void SetPasswordIsEmpty();
	void SetRegistSuccess();
	void SetRegistFail_IDNotUnique();
	void SetRegistFail_PasswordNotMatch();
	void SetRegistFail_Unknown();
	void SetTryLoginLater();
};
