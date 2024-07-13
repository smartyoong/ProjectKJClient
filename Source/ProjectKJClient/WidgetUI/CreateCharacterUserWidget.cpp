// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateCharacterUserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "CreateCharacterUserWidgetEntry.h"
#include "LoginResultWidget.h"
#include "MainGameInstance.h"
#include "GamePacketList.h"
#include "LoginPacketList.h"

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
	if (InputString.Len() >= 16 || InputString.Len() <= 0)
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

void UCreateCharacterUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

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
		CharacterPresetListView->SetSelectionMode(ESelectionMode::Single);
		CharacterPresetListView->OnItemClicked().AddUObject(this, &UCreateCharacterUserWidget::OnListItemClick);
	}
	if (CharacterImage)
	{
		CharacterImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCreateCharacterUserWidget::OnListItemClick(UObject* Obj)
{
	int32 Index = CharacterPresetListView->GetIndexForItem(Obj);
	if (PresetImageMaterialList.IsValidIndex(Index))
	{
		CharacterImage->SetBrushFromMaterial(PresetImageMaterialList[Index]);
		CharacterImage->SetVisibility(ESlateVisibility::Visible);
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
		// 성공하면 닉네임 반드시 GameInstance에 저장할 것
		SendCreateCharacterInfo();
	}
	else
	{
		// 닉네임이 유효하지 않음
		if (ResultWidgetClass)
		{
			ResultWidget = CreateWidget<ULoginResultWidget>(GetWorld(), ResultWidgetClass);
			if (ResultWidget)
			{
				ResultWidget->SetInvalidNickname();
				AsyncTask(ENamedThreads::GameThread, [this]()
					{
						ResultWidget->AddToViewport();
					});
			}
		}
	}
}


void UCreateCharacterUserWidget::PopulateList()
{
	if (CharacterPresetListView)
	{
		CharacterPresetListView->ClearListItems();
		for (int i = 0; i < PresetImageMaterialList.Num(); ++i)
		{
			UCreateCharacterPresetData* PresetData = NewObject<UCreateCharacterPresetData>();
			PresetData->PresetMaterial = PresetImageMaterialList[i];
			PresetData->PresetName = PresetNameList[i];
			CharacterPresetListView->AddItem(PresetData);
		}
	}
}

void UCreateCharacterUserWidget::SendCreateCharacterInfo()
{
	const int MALE = 1;
	const int FEMALE = 0;
	FRequestCreateCharacterPacket Packet;
	Packet.Gender = IsMale ? MALE : FEMALE;
	Packet.AccountID = Cast<UMainGameInstance>(GetGameInstance())->GetAccountID();
	Packet.HashCode = Cast<UMainGameInstance>(GetGameInstance())->GetUserAuthHashCode();
	Packet.PresetID = CharacterPresetListView->GetIndexForItem(CharacterPresetListView->GetSelectedItem());
	Cast<UMainGameInstance>(GetGameInstance())->SendPacketToGameServer<FRequestCreateCharacterPacket>(GamePacketListID::REQUEST_CREATE_CHARACTER, Packet);
	
	// 이걸 지금 보내지말고, 위의 캐릭 생성 성공하면 보내자, 그러면, Auth체크도 자동으로 된다.
	FCreateNickNameRequestPacket LoginPacket;
	LoginPacket.NickName = NickNameEditTextBox->GetText().ToString();
	LoginPacket.AccountID = Cast<UMainGameInstance>(GetGameInstance())->GetAccountID();

}
