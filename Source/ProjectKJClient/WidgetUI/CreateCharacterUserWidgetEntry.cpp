// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateCharacterUserWidgetEntry.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInterface.h"

void UCreateCharacterUserWidgetEntry::NativeOnListItemObjectSet(UObject* Obj)
{
	//이거 나중에 위젯에서 데이터 정리해야함
	UCreateCharacterPresetData* PresetData = Cast<UCreateCharacterPresetData>(Obj);
	if (PresetData)
	{
		PresetImage->SetBrushFromMaterial(PresetData->PresetImageMaterial.Get());
		NameTextBlock->SetText(FText::FromString(PresetData->PresetName));
	}
}

void UCreateCharacterUserWidgetEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	BackgroundImage->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	// 형광 연두색으로 설정하기 위한 FLinearColor 생성
	FLinearColor FluorescentGreenColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);

	// UImage 위젯의 ColorAndOpacity 속성 설정
	BackgroundImage->SetColorAndOpacity(FluorescentGreenColor);
}
