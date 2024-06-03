// Fill out your copyright notice in the Description page of Project Settings.


#include "RegistAccountUserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "MyUserWidget.h"
#include "LoginPacketManager.h"
#include "ProjectKJClient/MainGameInstance.h"

void URegistAccountUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	// 버튼 설정
	if (IDButton != nullptr)
		IDButton->OnClicked.AddDynamic(this, &URegistAccountUserWidget::OnIDUniqueCheckButtonClicked);
	if (RegistButton != nullptr)
		RegistButton->OnClicked.AddDynamic(this, &URegistAccountUserWidget::OnRegistButtonClicked);
	if (IDTextBox != nullptr)
	{
		IDTextBox->OnTextCommitted.AddDynamic(this, &URegistAccountUserWidget::OnIDTextCommitted);
		IDTextBox->OnTextChanged.AddDynamic(this, &URegistAccountUserWidget::OnIDTextChange);
		IDTextBox->SetSelectAllTextWhenFocused(true);
	}
	if (PWTextBox != nullptr)
	{
		PWTextBox->OnTextCommitted.AddDynamic(this, &URegistAccountUserWidget::OnPasswordTextCommitted);
		PWTextBox->SetSelectAllTextWhenFocused(true);
	}
	if (PWCheckTextBox != nullptr)
	{
		PWCheckTextBox->OnTextCommitted.AddDynamic(this, &URegistAccountUserWidget::OnPasswordCheckTextCommitted);
		PWCheckTextBox->SetSelectAllTextWhenFocused(true);
	}
}

void URegistAccountUserWidget::OnIDUniqueCheckButtonClicked()
{
	if (ID.IsEmpty())
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				RegistAccountResultWidget = CreateWidget<ULoginResultWidget>(GetWorld(), RegistAccountResultWidgetClass);
				RegistAccountResultWidget->SetIDIsEmpty();
				RegistAccountResultWidget->AddToViewport();
			});
	}
	else
	{
		FIDUniqueCheckRequestPacket IDUniqueCheckPacket;
		IDUniqueCheckPacket.AccountID = ID;
		// 패킷 송신
		LoginPacketListID PacketID = LoginPacketListID::ID_UNIQUE_CHECK_REQUEST;
		GetWorld()->GetGameInstance<UMainGameInstance>()->SendPacketToLoginServer<FIDUniqueCheckRequestPacket>(PacketID, IDUniqueCheckPacket);
	}
}

void URegistAccountUserWidget::OnRegistButtonClicked()
{
	if (!IsIDUnique)
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				RegistAccountResultWidget = CreateWidget<ULoginResultWidget>(GetWorld(), RegistAccountResultWidgetClass);
				RegistAccountResultWidget->SetIDUniqueCheckFail();
				RegistAccountResultWidget->AddToViewport();
			});
		return;
	}
	if (Password.IsEmpty())
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				RegistAccountResultWidget = CreateWidget<ULoginResultWidget>(GetWorld(), RegistAccountResultWidgetClass);
				RegistAccountResultWidget-> SetPasswordIsEmpty();
				RegistAccountResultWidget->AddToViewport();
			});
		return;
	}
	if (Password != PasswordCheck)
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				RegistAccountResultWidget = CreateWidget<ULoginResultWidget>(GetWorld(), RegistAccountResultWidgetClass);
				RegistAccountResultWidget->SetRegistFail_PasswordNotMatch();
				RegistAccountResultWidget->AddToViewport();
			});
		return;
	}

	FRegistAccountRequestPacket RegistAccountPacket;
	RegistAccountPacket.AccountID = ID;
	RegistAccountPacket.Password = Password;
	// 패킷 송신
	LoginPacketListID PacketID = LoginPacketListID::REGIST_ACCOUNT_REQUEST;
	GetWorld()->GetGameInstance<UMainGameInstance>()->SendPacketToLoginServer<FRegistAccountRequestPacket>(PacketID, RegistAccountPacket);
}

void URegistAccountUserWidget::OnIDTextCommitted(const FText& text, ETextCommit::Type TextType)
{
	ID = text.ToString();
}

void URegistAccountUserWidget::OnIDTextChange(const FText& text)
{
	if (IsIDUnique)
		IsIDUnique = false;
}

void URegistAccountUserWidget::OnPasswordTextCommitted(const FText& text, ETextCommit::Type TextType)
{
	Password = text.ToString();
}

void URegistAccountUserWidget::OnPasswordCheckTextCommitted(const FText& text, ETextCommit::Type TextType)
{
	PasswordCheck = text.ToString();
}

void URegistAccountUserWidget::OnCancelButtonClicked()
{
	if (ParentWidget != nullptr)
	{
		ParentWidget->ShowWidgetItems();
	}

	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			RemoveFromParent();
			Destruct();
		});
}

void URegistAccountUserWidget::RegistSuccess()
{
	// 성공시에는 해당 창을 그냥 닫는다.
	// 팝업 메세지는 부모가 띄운다.
	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			RemoveFromParent();
			Destruct();
		});
}

void URegistAccountUserWidget::RegistFail(int Mode)
{
	// 실패시에는 실패 메세지를 띄운다.
	AsyncTask(ENamedThreads::GameThread, [this, Mode]()
		{
			RegistAccountResultWidget = CreateWidget<ULoginResultWidget>(GetWorld(), RegistAccountResultWidgetClass);
			switch (Mode)
			{
			case -1:
				RegistAccountResultWidget->SetRegistFail_IDNotUnique();
				break;
			case -2:
				RegistAccountResultWidget->SetRegistFail_Unknown();
				break;
			default:
				break;
			}
			RegistAccountResultWidget->AddToViewport();
		});
}
