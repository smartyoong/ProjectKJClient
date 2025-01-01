// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetUI/TargetInterfaceWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UTargetInterfaceWidget::SetTargetInfo(const APawn& Object)
{

}

void UTargetInterfaceWidget::SetHPBarPercent(int CurrentHP, int MaxHP)
{
	if (MaxHP <= 0)
	{
		return;
	}

	if (HPProgressBar != nullptr)
	{
		HPProgressBar->SetPercent(static_cast<float>(CurrentHP) / static_cast<float>(MaxHP));
	}
}

void UTargetInterfaceWidget::SetNameText(FString Name)
{
	if (NameTextBlock != nullptr)
	{
		NameTextBlock->SetText(FText::FromString(Name));
	}
}

void UTargetInterfaceWidget::SetLevelText(int Level)
{
	if (LevelTextBlock != nullptr)
	{
		LevelTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), Level)));
	}
}

void UTargetInterfaceWidget::SetCharacterInfo(const APlayerCharacter& Object)
{
}

void UTargetInterfaceWidget::SetNormalMonsterInfo(const ANormalMonster& Object)
{
}
