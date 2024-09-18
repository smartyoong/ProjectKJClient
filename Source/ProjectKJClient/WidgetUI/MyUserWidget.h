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
	bool ButtonClicked = false;
	FString ID;
	FString Password;
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
	UPROPERTY()
	class URegistAccountUserWidget* RegistAccountWidget;
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnLoginButtonClicked();

	UFUNCTION()
	void OnRegistButtonClicked();

	UFUNCTION()
	void OnIDTextCommitted(const FText& text, ETextCommit::Type TextType);

	UFUNCTION()
	void OnPasswordTextCommitted(const FText& text, ETextCommit::Type TextType);

	UFUNCTION()
	void OnPasswordChanged(const FText& text);
	
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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class URegistAccountUserWidget> RegistAccountWidgetClass;

	void ShowLoginResultWidget(int Mode);

	void ShowRegistAccountWidget();

	void ShowWidgetItems();

	void ShowRegistSuccessPopUp();

	void ShowLoginSuccess();

	void ShowLoginFail();

	void ShowKickClient(int Reason);

	class URegistAccountUserWidget* GetRegistAccountWidget();
};
