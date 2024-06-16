// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreenWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

void ULoadingScreenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
    if(MainImage != nullptr)
        MainImage->SetVisibility(ESlateVisibility::Hidden);
	SelectRandomImage();
}

void ULoadingScreenWidget::SelectRandomImage()
{

    // 랜덤 인덱스 선택
    int32 RandomIndex = FMath::RandRange(0, 2);
    FSlateBrush MyBrush;
    switch (RandomIndex)
    {
        case 0:
            if (LoadingImage1 != nullptr)
            {
                // UImage 컴포넌트에 이미지 설정
                MyBrush.SetResourceObject(LoadingImage1);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("이미지1 로드 실패"));
            }
		    break;
        case 1:
            if (LoadingImage2 != nullptr)
            {
                MyBrush.SetResourceObject(LoadingImage2);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("이미지2 로드 실패"));
            }
            break;
        case 2:
            if (LoadingImage3 != nullptr)
            {
                MyBrush.SetResourceObject(LoadingImage3);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("이미지3 로드 실패"));
            }
            break;
        default:
            break;
    }
    if(MainImage != nullptr)
	{
		MainImage->SetBrush(MyBrush);
        MainImage->SetVisibility(ESlateVisibility::Visible);
	}
    else
    {
        UE_LOG(LogTemp, Error, TEXT("MainImage 로드 실패"));
    }
}
