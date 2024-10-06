// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PROJECTKJCLIENT_API PathComponent
{
private:
	TArray<FVector> Positions;
	int CurrentPositionIndex = 0;
	int TotalSize = 0;
public:
	PathComponent() = default;
	void AddPosition(FVector Position);
	int GetCurrentIndex();
	FVector GetPosition(int Index);
	int GetNextPositionIndex();
	bool Arrived(FVector CharacterPosition);
};
