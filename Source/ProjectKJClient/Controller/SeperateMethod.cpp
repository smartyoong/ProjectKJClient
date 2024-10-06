// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeperateMethod.h"

SeperateMethod::SeperateMethod(TArray<KinematicStatic>* TargetList) : Targets(TargetList)
{
}

std::optional<SteeringHandle> SeperateMethod::GetSteeringHandle(float Ratio, KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate, float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget)
{
	if (Targets == nullptr)
		return std::optional<SteeringHandle>();

	if (Targets->Num() == 0)
		return std::optional<SteeringHandle>();

	SteeringHandle Result{FVector::ZeroVector,0.f};
	
	for (auto& t : *Targets)
	{
		FVector Direction = Character.Position - t.Position;
		float Distance = Direction.Size();
		if (Distance < Threshold)
		{
			float Strength = FMath::Min(DecayCoefficient / (Distance * Distance), MaxSpeed);
			Direction.Normalize();
			Result.Linear += Strength * Direction;
		}
	}
	return Result;
}
