// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetUI/MainHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMainHUDWidget::SetHP(int HP, int MaxHP)
{
	if (MaxHP <= 0)
	{
		return;
	}

	if(HPProgressBar != nullptr)
	{
		HPProgressBar->SetPercent(static_cast<float>(HP) / static_cast<float>(MaxHP));
	}

	if (HPTextBlock != nullptr)
	{
		HPTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), HP, MaxHP)));
	}
}

void UMainHUDWidget::SetMP(int MP, int MaxMP)
{
	if (MaxMP <= 0)
	{
		return;
	}
	if (MPProgressBar != nullptr)
	{
		MPProgressBar->SetPercent(static_cast<float>(MP) / static_cast<float>(MaxMP));
	}
	if (MPTextBlock != nullptr)
	{
		MPTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), MP, MaxMP)));
	}
}
