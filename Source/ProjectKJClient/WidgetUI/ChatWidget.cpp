// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetUI/ChatWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/Border.h"
#include "ProjectKJClientPlayerController.h"
#include "PacketList/GamePacketList.h"
#include "MainGameInstance.h"



// 추후 채팅 기능 만들때 UTF-8을 UTF-16으로 정상적으로 바꾸는 방법
//// UTF-8 인코딩된 문자열을 FString으로 변환하는 예시
//TArray<uint8> UTF8Data; // UTF-8 인코딩된 데이터를 포함하는 배열
//UTF8Data.Add(0); // 널 종료 문자 추가 (이건 없애도 될 수도 있음 내가 언리얼 코드 조작하면서 널문자 제거 안하도록 바꿈)
//
//// UTF-8 데이터를 FString으로 변환
//FString ChatMessage = UTF8_TO_TCHAR(reinterpret_cast<const char*>(UTF8Data.GetData()));
//
//// FString을 FText로 변환 (UI에 표시하기 위해)
//FText ChatMessageText = FText::FromString(ChatMessage);
//
//// UEditableTextBox에 텍스트 지정
//UEditableTextBox* MyEditableTextBox = ...; // UEditableTextBox 인스턴스를 얻는 방법은 상황에 따라 다름
//MyEditableTextBox->SetText(ChatMessageText);

void UChatWidget::NativeConstruct()
{
	if(ChatSendButton != nullptr)
		ChatSendButton->OnClicked.AddDynamic(this, &UChatWidget::OnChatSendButtonClicked);
	if (ChatModeButton != nullptr)
		ChatModeButton->OnClicked.AddDynamic(this, &UChatWidget::OnChatModeButtonClicked);
	if (ChatEditableTextBox != nullptr)
	{
		ChatEditableTextBox->SetClearKeyboardFocusOnCommit(true);
		ChatEditableTextBox->OnTextCommitted.AddDynamic(this, &UChatWidget::OnChatEditableTextBoxCommitted);
	}

	if (ChatSendButton)
		ChatSendButton->SetIsEnabled(false);
}

void UChatWidget::OnChatSendButtonClicked()
{
	if (!ChatEditableTextBox) return;

	if (ChatSendButton)
		ChatSendButton->SetIsEnabled(false);

	IsChatting = false;

	if (ChatEditableTextBox->GetText().IsEmpty())
	{
		return;
	}

	FString Message;
	switch (ChatMode)
	{
		case EChatMode::MAP:
			Message = "";
			break;
		case EChatMode::ALL:
			Message = "/a";
			break;
		case EChatMode::FRIEND:
			Message = "/f";
			break;
		default:
			break;
	}
	Message += " ";
	// 추후에 귓속말 만들때 여기를 상대방으로 지정해야함, 그 외에는 나의 닉네임을 보내도록 처리
	Message += NickName;
	Message += " ";
	Message += ChatEditableTextBox->GetText().ToString();
	// C#은 UTF-8 인코딩을 사용하고 있음 언리얼을 UTF-16인데 이걸 UTF-8로 바꿔서 보내야함
	FTCHARToUTF8 UTF8String(*Message);
	FString PacketMessage = UTF8String.Get();

	FRequestUserSayPacket ChatPacket;
	ChatPacket.AccountID = Cast<UMainGameInstance>(GetGameInstance())->GetAccountID();
	ChatPacket.Message = PacketMessage;
	Cast<UMainGameInstance>(GetGameInstance())->SendPacketToGameServer<FRequestUserSayPacket>(GamePacketListID::REQUEST_USER_SAY, ChatPacket);

	ChatEditableTextBox->SetText(FText::FromString(TEXT("")));
	ChatEditableTextBox->SetIsEnabled(false);

	// TODO : 채팅 패킷 전송, 채팅 엔터 연속으로 입력안되는거 확인필요
	//PlayerController->SetMouseCaptureMode(EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
	//bEnableClickEvents = true; 를 활성화하여 마우스 입력이 게임에서 처리되도록 설정
	FSlateApplication::Get().SetUserFocusToGameViewport(0); //를 호출하여 강제로 게임으로 포커스를 이동 이게 있어?
}

void UChatWidget::OnChatEditableTextBoxCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		OnChatSendButtonClicked();
	}
}

void UChatWidget::SetFocusToChatEditableTextBox()
{
	IsChatting = true;

	if (ChatEditableTextBox)
	{
		ChatEditableTextBox->SetIsEnabled(true);
		ChatEditableTextBox->SetKeyboardFocus();
	}

	if (ChatSendButton)
	{
		ChatSendButton->SetIsEnabled(true);
	}
}

bool UChatWidget::IsChattingNow()
{
	return IsChatting;
}

void UChatWidget::OnChatModeButtonClicked()
{
	ChatMode = static_cast<EChatMode>((static_cast<uint8>(ChatMode) + 1) % static_cast<uint8>(EChatMode::MAX));
	switch (ChatMode)
	{
	case EChatMode::MAP:
		ChatModeTextBlock->SetText(FText::FromString(TEXT("맵")));
		break;
	case EChatMode::ALL:
		ChatModeTextBlock->SetText(FText::FromString(TEXT("전체")));
		break;
	case EChatMode::FRIEND:
		ChatModeTextBlock->SetText(FText::FromString(TEXT("친구")));
		break;
	default:
		break;
	}
	SetFocusToChatEditableTextBox();
}

void UChatWidget::RemoveOldestChatMessage()
{
	if (ChatVerticalBox->GetChildrenCount() > MAX_CHAT_COUNT)
	{
		ChatVerticalBox->RemoveChildAt(0);
	}
}

void UChatWidget::ClearChatMessage()
{
	ChatVerticalBox->ClearChildren();
}

void UChatWidget::SetTextColorByMode(UTextBlock& Text, int32 ChatType)
{
	EChatMode ChatTypeMode = static_cast<EChatMode>(ChatType);
	switch (ChatTypeMode)
	{
	case EChatMode::MAP:
		Text.SetColorAndOpacity(FSlateColor(FLinearColor::White));
		break;
	case EChatMode::ALL:
		Text.SetColorAndOpacity(FSlateColor(FLinearColor::Green));
		break;
	case EChatMode::FRIEND:
		Text.SetColorAndOpacity(FSlateColor(FLinearColor::Blue));
		break;
	default:
		// 잘못왔다는 것을 명시하기 위함
		Text.SetColorAndOpacity(FSlateColor(FLinearColor::Black));
		break;
	}
}

void UChatWidget::AddChatMessage(const FString& SenderNickName, const FString& Message, int32 ChatType)
{

	if (!ChatVerticalBox) return;

	// 출력에 문제가 생기면 여기 수정 필요
	FString ChatMessage = SenderNickName + TEXT(" : ") + Message;

	UBorder* MessageBorder = NewObject<UBorder>(this);
	if (MessageBorder)
	{
		// 배경색 설정 (하얀색, 알파 0.5)
		MessageBorder->SetBrushColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));

		UTextBlock* NewMessage = NewObject<UTextBlock>(this);
		if (NewMessage)
		{
			NewMessage->SetText(FText::FromString(ChatMessage));
			SetTextColorByMode(*NewMessage, ChatType);

			// UI에 출력할거면 메인스레드에서 작업해야함
			AsyncTask(ENamedThreads::GameThread, [this, MessageBorder, NewMessage]()
				{
					MessageBorder->AddChild(NewMessage);
					ChatVerticalBox->AddChild(MessageBorder);
					ChatScrollBox->ScrollToEnd();
					RemoveOldestChatMessage();
					FSlateApplication::Get().SetUserFocusToGameViewport(0); //를 호출하여 강제로 게임으로 포커스를 이동 이게 있어?
				});
		}
	}
}

void UChatWidget::ChatShortcutAction()
{
	if (!IsChattingNow())
	{
		SetFocusToChatEditableTextBox();
	}
}
