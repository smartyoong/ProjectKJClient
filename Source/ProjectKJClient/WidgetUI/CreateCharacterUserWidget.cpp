// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateCharacterUserWidget.h"
#include "Components/Image.h"
#include "Components/ListView.h"

void UCreateCharacterUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (CharacterPresetListView)
	{
		CharacterPresetListView->OnItemClicked().AddUObject(this, &UCreateCharacterUserWidget::OnListItemClick);
		// 리스트에 있는 아이템을 EntryList에 추가하자
		// TArray에 있는것들 추가필요
		CharacterPresetListView->SetListItems(PresetDataList);
	}
	if (CharacterImage)
	{
		CharacterImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCreateCharacterUserWidget::OnListItemClick(UObject* Obj)
{
	//여기서 Obj가 위에 SetListItems에서 넣은 데이터중 1개임
	if(CharacterImage)
		CharacterImage->SetVisibility(ESlateVisibility::Hidden);

	UCreateCharacterPresetData* PresetData = Cast<UCreateCharacterPresetData>(Obj);
	if (PresetData)
	{
		if (CharacterImage)
		{
			CharacterImage->SetBrushFromMaterial(PresetData->PresetImageMaterial.Get());
			CharacterImage->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
