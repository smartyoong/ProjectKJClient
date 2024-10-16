// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PathComponent.h"

void PathComponent::AddPosition(FVector Position)
{
	FScopeLock Lock(&CriticalSection);
	Positions.Add(Position);
	TotalSize++;
}

int PathComponent::GetCurrentIndex()
{
	FScopeLock Lock(&CriticalSection);
	return CurrentPositionIndex;
}

FVector PathComponent::GetPosition(int Index)
{
	FScopeLock Lock(&CriticalSection);
	return Positions[Index];
}

int PathComponent::GetNextPositionIndex()
{
	FScopeLock Lock(&CriticalSection);

	if (CurrentPositionIndex == -1)
	{
		CurrentPositionIndex = 0; // 다시 처음으로 돌린다.
		return 0;
	}
	CurrentPositionIndex++;

	if (CurrentPositionIndex >= TotalSize)
	{
		// 다음 위치가 없다.
		CurrentPositionIndex = -1;
	}
	return CurrentPositionIndex;
}

bool PathComponent::Arrived(FVector CharacterPosition)
{
	FScopeLock Lock(&CriticalSection);

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

void PathComponent::SetPathPoints(TArray<FVector> PathPoints)
{
	FScopeLock Lock(&CriticalSection);
	Positions = PathPoints;
	TotalSize = Positions.Num();
	CurrentPositionIndex = 0;
}
