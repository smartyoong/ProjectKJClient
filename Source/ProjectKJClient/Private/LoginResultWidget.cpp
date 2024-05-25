// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginResultWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void ULoginResultWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (OKButton != nullptr)
		OKButton->OnClicked.AddDynamic(this, &ULoginResultWidget::OnOkButtonClicked);

}

void ULoginResultWidget::OnOkButtonClicked()
{
	RemoveFromParent();
	Destruct();
}

void ULoginResultWidget::SetLoginSuceess()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("로그인 성공")));
}

void ULoginResultWidget::SetNoAccount()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("존재하지 않는 계정.")));
}

void ULoginResultWidget::SetPasswordFail()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("비밀번호가 틀렸습니다.")));
}
