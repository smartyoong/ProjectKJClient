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
    UCreateCharacterPresetData* PresetData = Cast<UCreateCharacterPresetData>(Obj);
    if (IsValid(PresetData))
    {
        NameTextBlock->SetText(FText::FromString(PresetData->PresetName));

        if (IsValid(PresetData->PresetImage2DTexture))
        {
            PresetImage->SetBrushFromTexture(PresetData->PresetImage2DTexture);
            PresetImage->SetVisibility(ESlateVisibility::Visible);
            UE_LOG(LogTemp, Warning, TEXT("Texture set successfully. Texture size: %d x %d"),
                PresetData->PresetImage2DTexture->GetSizeX(),
                PresetData->PresetImage2DTexture->GetSizeY());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Preset Image texture is not valid"));
            PresetImage->SetVisibility(ESlateVisibility::Hidden);
        }

        UE_LOG(LogTemp, Warning, TEXT("Entry Set Success for Preset: %s"), *PresetData->PresetName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Entry Set Fail: Invalid PresetData"));
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
