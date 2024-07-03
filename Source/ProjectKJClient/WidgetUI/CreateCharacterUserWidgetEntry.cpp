// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateCharacterUserWidgetEntry.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInterface.h"

void UCreateCharacterUserWidgetEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UE_LOG(LogTemp, Warning, TEXT("Entry Initialized"));
	IsFocusable();
}

void UCreateCharacterUserWidgetEntry::NativeOnListItemObjectSet(UObject* Obj)
{
	//이거 나중에 위젯에서 데이터 정리해야함
	UCreateCharacterPresetData* PresetData = Cast<UCreateCharacterPresetData>(Obj);
	BackgroundImage->SetVisibility(ESlateVisibility::Hidden);
	PresetImage->SetVisibility(ESlateVisibility::Hidden);
	FSlateBrush MyBrush;
	if (PresetData)
	{
		MyBrush.SetResourceObject(PresetData->PresetImage2DTexture);
		PresetImage->SetBrush(MyBrush);
		NameTextBlock->SetText(FText::FromString(PresetData->PresetName));
		PresetImage->SetVisibility(ESlateVisibility::Visible);
		UE_LOG(LogTemp, Warning, TEXT("Entry Image Visible"));
	}
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
