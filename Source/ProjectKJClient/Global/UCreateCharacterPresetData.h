// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UCreateCharacterPresetData.generated.h"

/**
 * 
 */
UCLASS()
class UCreateCharacterPresetData : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UMaterialInterface* PresetMaterial;
	FString PresetName = "Sample";
};