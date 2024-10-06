// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PathComponent.h"

void PathComponent::AddPosition(FVector Position)
{
	Positions.Add(Position);
	TotalSize++;
}

int PathComponent::GetCurrentIndex()
{
	return CurrentPositionIndex;
}

FVector PathComponent::GetPosition(int Index)
{
	return Positions[Index];
}

int PathComponent::GetNextPositionIndex()
{
	CurrentPositionIndex++;
	CurrentPositionIndex %= TotalSize;
	return CurrentPositionIndex;
}

bool PathComponent::Arrived(FVector CharacterPosition)
{
	if(TotalSize == 0)
	{
		return true;
	}

	FVector CurrentPosition = Positions[CurrentPositionIndex];

	float Distance = FVector::Dist(CharacterPosition, CurrentPosition);
	if (Distance < 3.f)
	{
		return true;
	}
	return false;
}
