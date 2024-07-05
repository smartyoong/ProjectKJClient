// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateCharacterUserWidgetEntry.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInterface.h"

void UCreateCharacterUserWidgetEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BackgroundImage->SetVisibility(ESlateVisibility::Hidden);
}

void UCreateCharacterUserWidgetEntry::NativeOnListItemObjectSet(UObject* Obj)
{
	//이거 나중에 위젯에서 데이터 정리해야함
	UCreateCharacterPresetData* PresetData = Cast<UCreateCharacterPresetData>(Obj);
	if (IsValid(PresetData))
	{
		NameTextBlock->SetText(FText::FromString(PresetData->PresetName));
		if(IsValid(PresetData->PresetImage2DTexture))
			PresetImage->SetBrushFromTexture(PresetData->PresetImage2DTexture);
		else
			UE_LOG(LogTemp, Error, TEXT("Preset Image is not valid"));
		PresetImage->SetVisibility(ESlateVisibility::Visible);
		UE_LOG(LogTemp, Warning, TEXT("Entry Set Success"));
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Entry Set Fail"));
}

void UCreateCharacterUserWidgetEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	// 형광 연두색으로 설정하기 위한 FLinearColor 생성
	FLinearColor FluorescentGreenColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);

	// UImage 위젯의 ColorAndOpacity 속성 설정
	BackgroundImage->SetColorAndOpacity(FluorescentGreenColor);
	BackgroundImage->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	UE_LOG(LogTemp, Warning, TEXT("Entry Selection Changed"));
}
