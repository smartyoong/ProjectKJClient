// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MediaTexture.h"
#include "MediaPlayer.h"
#include "MediaPlaylist.h"
#include "MediaSoundComponent.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKJCLIENT_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	FString ID;
	FString Password;
	bool IsFirstTry = true;
	bool IsFirstPWTry = true;
	UPROPERTY()
	UMediaTexture* MediaTexture = nullptr;
	UPROPERTY()
	UMediaPlayer* MediaPlayer = nullptr;
	UPROPERTY()
	UMediaPlaylist* Playlist = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Media")
	USoundBase* SoundToPlay = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Media")
	class UMediaSource* MediaSource = nullptr;

	UPROPERTY()
	class ULoginResultWidget* LoginResultWidget;
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnLoginButtonClicked();

	UFUNCTION()
	void OnRegistButtonClicked();

	UFUNCTION()
	void OnIDTextChanged(const FText& text);

	UFUNCTION()
	void OnPasswordTextChanged(const FText& text);
	
public:

	UPROPERTY( meta = (BindWidget))
	class UButton* LoginButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* RegistButton;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* IDTextBox;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* PasswordTextBox;

	UPROPERTY(meta = (BindWidget))
	class UImage* MainImage;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ULoginResultWidget> LoginResultWidgetClass;

	void ShowLoginResultWidget(int Mode);
};
