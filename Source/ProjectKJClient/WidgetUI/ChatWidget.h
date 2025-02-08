// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatWidget.generated.h"

/**
 * 
 */

enum class EChatMode : uint8
{
	MAP,
	ALL,
	FRIEND,
	WHISPER,
	PARTY,
	MAX
};

const constexpr int MAX_CHAT_COUNT = 100;

UCLASS()
class PROJECTKJCLIENT_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	FString NickName;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ChatEditableTextBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* ChatSendButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ChatModeButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ChatModeTextBlock;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ChatVerticalBox;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ChatScrollBox;

	bool IsChatting = false;
	
protected:
	virtual void NativeConstruct() override;
private:

	EChatMode ChatMode = EChatMode::MAP;

	UFUNCTION()
	void OnChatModeButtonClicked();
	void RemoveOldestChatMessage();
	void ClearChatMessage();
	void SetTextColorByMode(class UTextBlock& Text, int32 ChatType);
	UFUNCTION()
	void OnChatSendButtonClicked();
	UFUNCTION()
	void OnChatEditableTextBoxCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	void SetFocusToChatEditableTextBox();
	bool IsChattingNow();
public:
	void AddChatMessage(const FString& SenderNickName, const FString& Message, int32 ChatType);
	void SetNickName(const FString& InNickName) { NickName = InNickName; }
	void ChatShortcutAction();
};
