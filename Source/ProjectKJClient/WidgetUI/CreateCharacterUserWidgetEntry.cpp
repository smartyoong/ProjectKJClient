// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateCharacterUserWidgetEntry.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInterface.h"


void UCreateCharacterUserWidgetEntry::NativeOnListItemObjectSet(UObject* Obj)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(Obj);
    UCreateCharacterPresetData* PresetData = Cast<UCreateCharacterPresetData>(Obj);
    if (IsValid(PresetData))
    {
		UpdateEntry(PresetData->PresetImage2DTexture, PresetData->PresetName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Entry Set Fail: Invalid PresetData"));
    }
}

void UCreateCharacterUserWidgetEntry::NativeConstruct()
{
	Super::NativeConstruct();
	BackgroundImage->SetVisibility(ESlateVisibility::Hidden);
}

void UCreateCharacterUserWidgetEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
	// 형광 연두색으로 설정하기 위한 FLinearColor 생성
	FLinearColor FluorescentGreenColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);

	// UImage 위젯의 ColorAndOpacity 속성 설정
	BackgroundImage->SetColorAndOpacity(FluorescentGreenColor);
	BackgroundImage->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	UE_LOG(LogTemp, Warning, TEXT("Entry Selection Changed"));
}

void UCreateCharacterUserWidgetEntry::UpdateEntry(UTexture2D* ImageData, FString TextData)
{
	NameTextBlock->SetText(FText::FromString(TextData));

	if (IsValid(ImageData))
	{
		FSlateBrush MyBrush;
		ImageData->UpdateResource();
		MyBrush.SetResourceObject(ImageData);
		PresetImage->SetBrush(MyBrush);
		PresetImage->SetVisibility(ESlateVisibility::Visible);
		UE_LOG(LogTemp, Warning, TEXT("Preset Image texture is valid"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Preset Image texture is not valid"));
		PresetImage->SetVisibility(ESlateVisibility::Hidden);
	}
}
