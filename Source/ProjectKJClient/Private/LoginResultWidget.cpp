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

void ULoginResultWidget::SetIDIsEmpty()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("ID를 입력해주세요.")));
}

void ULoginResultWidget::SetIDUniqueCheckFail()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("사용중인 ID")));
}

void ULoginResultWidget::SetIDUniqueCheckSuccess()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("사용가능한 ID")));
}

void ULoginResultWidget::SetPasswordIsEmpty()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("비밀번호를 입력해주세요.")));
}

void ULoginResultWidget::SetRegistSuccess()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("회원가입 성공")));
}

void ULoginResultWidget::SetRegistFail_IDNotUnique()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("사용중인 ID")));
}

void ULoginResultWidget::SetRegistFail_PasswordNotMatch()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("비밀번호가 일치하지 않습니다.")));
}

void ULoginResultWidget::SetRegistFail_Unknown()
{
	if (ResultTextBlock != nullptr)
		ResultTextBlock->SetText(FText::FromString(TEXT("알 수 없는 오류")));
}




