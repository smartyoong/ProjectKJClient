// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class UCreateCharacterPresetData : public UObject
{
public:
	UPROPERTY()
	UMaterialInterface * PresetImageMaterial;
	FString PresetName;
};