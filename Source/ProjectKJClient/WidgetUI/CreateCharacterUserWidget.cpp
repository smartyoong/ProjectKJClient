// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateCharacterUserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"

bool UCreateCharacterUserWidget::IsKoreanOrAlphaNumeric(TCHAR Char)
{

	// 영어 알파벳이나 숫자인지 체크
	if (FChar::IsAlnum(Char))
	{
		return true;
	}

	// 한국어 문자 범위 체크 (가 ~ 힣)
	int32 CharCode = Char;
	if (CharCode >= 0xAC00 && CharCode <= 0xD7A3)
	{
		return true;
	}

	// 여기에 다른 언어의 문자 범위를 추가할 수 있습니다.

	return false;

}

bool UCreateCharacterUserWidget::IsNickNameValid(const FString& InputString)
{
	// 문자열 길이 체크
	if (InputString.Len() >= 50)
	{
		return false;
	}

	// 문자열을 순회하면서 각 문자가 알파벳이거나 숫자인지 체크
	for (TCHAR Char : InputString)
	{
		if (!IsKoreanOrAlphaNumeric(Char))
		{
			return false;
		}
	}

	// 모든 체크를 통과했다면 true 반환
	return true;
}

void UCreateCharacterUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (OKButton)
	{
		OKButton->OnClicked.AddDynamic(this, &UCreateCharacterUserWidget::OnOKButtonClick);
	}
	if (GenderButton)
	{
		GenderButton->OnClicked.AddDynamic(this, &UCreateCharacterUserWidget::OnGenderButtonClick);
	}
	if (NickNameEditTextBox)
	{
		NickNameEditTextBox->SetSelectAllTextWhenFocused(true);
	}
	if (CharacterPresetListView)
	{
		CharacterPresetListView->OnItemClicked().AddUObject(this, &UCreateCharacterUserWidget::OnListItemClick);
		for(int i = 0 ; i < PresetImageMaterialList.Num(); ++i)
		{
			if (PresetImageMaterialList[i])
			{
				UCreateCharacterPresetData* PresetData = NewObject<UCreateCharacterPresetData>();
				PresetData->PresetName = PresetNameList[i];
				PresetData->PresetImageMaterial = PresetImageMaterialList[i];
				PresetDataList.Add(PresetData);
			}
		}
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
			CharacterImage->SetBrushFromMaterial(PresetData->PresetImageMaterial);
			CharacterImage->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UCreateCharacterUserWidget::OnGenderButtonClick()
{
	// 추후 작업할것
	if (IsMale)
	{
		GenderTextBlock->SetText(FText::FromString(TEXT("여자")));
		IsMale = !IsMale;
	}
	else
	{
		GenderTextBlock->SetText(FText::FromString(TEXT("남자")));
		IsMale = !IsMale;
	}
}

void UCreateCharacterUserWidget::OnOKButtonClick()
{
	// 서버랑 연계 작업할 것
	if(IsNickNameValid(NickNameEditTextBox->GetText().ToString()))
	{
		// 서버로 닉네임 전송
	}
	else
	{
		// 닉네임이 유효하지 않습니다.
		UE_LOG(LogTemp, Error, TEXT("닉네임이 유효하지 않습니다."));
	}
}
