// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/LookAtToMoveMethod.h"
#include "AlignMethod.h"

std::optional<SteeringHandle> LookAtToMoveMethod::GetSteeringHandle(float Ratio, KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate, float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget)
{
	FVector Velocity = Character.Velocity;
	if (Velocity.Length() == 0)
	{
		return std::nullopt;
	}
	KinematicStatic NewTarget = Target;
	NewTarget.Orientation = FMath::Atan2(Velocity.Y, Velocity.X);
	return AlignMethod().GetSteeringHandle(Ratio, Character, NewTarget, MaxSpeed, MaxAccelerate, MaxRotation, MaxAngular, TargetRadius, SlowRadius, TimeToTarget);
}
