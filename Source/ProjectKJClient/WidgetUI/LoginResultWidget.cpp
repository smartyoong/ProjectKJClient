// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginResultWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Throbber.h"

void ULoginResultWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (OKButton != nullptr)
		OKButton->OnClicked.AddDynamic(this, &ULoginResultWidget::OnOkButtonClicked);
	if(ConnectThrobber != nullptr)
		ConnectThrobber->SetVisibility(ESlateVisibility::Hidden);

}

void ULoginResultWidget::OnOkButtonClicked()
{
	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			RemoveFromParent();
			Destruct();
		});
}

void ULoginResultWidget::SetLoginSuceess()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("로그인 성공")));
	if (ConnectThrobber != nullptr)
		ConnectThrobber->SetVisibility(ESlateVisibility::Hidden);
	if (OKButton != nullptr)
	{
		OKButton->SetIsEnabled(true);
		OKButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULoginResultWidget::SetNoAccount()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("존재하지 않는 계정.")));
	if (ConnectThrobber != nullptr)
		ConnectThrobber->SetVisibility(ESlateVisibility::Hidden);
	if (OKButton != nullptr)
	{
		OKButton->SetIsEnabled(true);
		OKButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULoginResultWidget::SetPasswordFail()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("비밀번호가 틀렸습니다.")));
	if (ConnectThrobber != nullptr)
		ConnectThrobber->SetVisibility(ESlateVisibility::Hidden);
	if (OKButton != nullptr)
	{
		OKButton->SetIsEnabled(true);
		OKButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULoginResultWidget::SetIDIsEmpty()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("ID를 입력해주세요.")));
	if (ConnectThrobber != nullptr)
		ConnectThrobber->SetVisibility(ESlateVisibility::Hidden);
	if (OKButton != nullptr)
	{
		OKButton->SetIsEnabled(true);
		OKButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULoginResultWidget::SetIDUniqueCheckFail()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("사용중인 ID")));
	if (ConnectThrobber != nullptr)
		ConnectThrobber->SetVisibility(ESlateVisibility::Hidden);
	if (OKButton != nullptr)
	{
		OKButton->SetIsEnabled(true);
		OKButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULoginResultWidget::SetIDUniqueCheckSuccess()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("사용가능한 ID")));
	if (ConnectThrobber != nullptr)
		ConnectThrobber->SetVisibility(ESlateVisibility::Hidden);
	if (OKButton != nullptr)
	{
		OKButton->SetIsEnabled(true);
		OKButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULoginResultWidget::SetPasswordIsEmpty()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("비밀번호를 입력해주세요.")));
	if (ConnectThrobber != nullptr)
		ConnectThrobber->SetVisibility(ESlateVisibility::Hidden);
	if (OKButton != nullptr)
	{
		OKButton->SetIsEnabled(true);
		OKButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULoginResultWidget::SetRegistSuccess()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("회원가입 성공")));
	if (ConnectThrobber != nullptr)
		ConnectThrobber->SetVisibility(ESlateVisibility::Hidden);
	if (OKButton != nullptr)
	{
		OKButton->SetIsEnabled(true);
		OKButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULoginResultWidget::SetRegistFail_IDNotUnique()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("사용중인 ID")));
	if (ConnectThrobber != nullptr)
		ConnectThrobber->SetVisibility(ESlateVisibility::Hidden);
	if (OKButton != nullptr)
	{
		OKButton->SetIsEnabled(true);
		OKButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULoginResultWidget::SetRegistFail_PasswordNotMatch()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("비밀번호 불일치")));
	if (ConnectThrobber != nullptr)
		ConnectThrobber->SetVisibility(ESlateVisibility::Hidden);
	if (OKButton != nullptr)
	{
		OKButton->SetIsEnabled(true);
		OKButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULoginResultWidget::SetRegistFail_Unknown()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("알 수 없는 오류")));
	if (ConnectThrobber != nullptr)
		ConnectThrobber->SetVisibility(ESlateVisibility::Hidden);
	if (OKButton != nullptr)
	{
		OKButton->SetIsEnabled(true);
		OKButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULoginResultWidget::SetTryLoginLater()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("잠시 후 다시 시도해주세요.")));
	if (ConnectThrobber != nullptr)
		ConnectThrobber->SetVisibility(ESlateVisibility::Hidden);
	if (OKButton != nullptr)
	{
		OKButton->SetIsEnabled(true);
		OKButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULoginResultWidget::SetLoginConnect()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("서버에 연결중입니다.")));
	if(ConnectThrobber != nullptr)
		ConnectThrobber->SetVisibility(ESlateVisibility::Visible);
	if (OKButton != nullptr)
	{
		OKButton->SetIsEnabled(false);
		OKButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ULoginResultWidget::ShowKickMessage(int Reason)
{
	switch (Reason)
	{
	case 1:
		if (ResultTextBlock != nullptr)
			ResultTextBlock->SetText(FText::FromString(TEXT("다른 유저가 접속을 시도했습니다.")));
			break;
	default:
		break;
	}

	if (ConnectThrobber != nullptr)
		ConnectThrobber->SetVisibility(ESlateVisibility::Hidden);
	if (OKButton != nullptr)
	{
		OKButton->SetIsEnabled(true);
		OKButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULoginResultWidget::SetInvalidNickname()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("사용할 수 없는 닉네임")));
	if (ConnectThrobber != nullptr)
		ConnectThrobber->SetVisibility(ESlateVisibility::Hidden);
	if (OKButton != nullptr)
	{
		OKButton->SetIsEnabled(true);
		OKButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULoginResultWidget::SetCreateCharacterFail()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("캐릭터 생성 실패")));
	if (ConnectThrobber != nullptr)
		ConnectThrobber->SetVisibility(ESlateVisibility::Hidden);
	if (OKButton != nullptr)
	{
		OKButton->SetIsEnabled(true);
		OKButton->SetVisibility(ESlateVisibility::Visible);
	}
}






