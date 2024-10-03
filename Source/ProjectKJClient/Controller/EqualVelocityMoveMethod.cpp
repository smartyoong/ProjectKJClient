// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/EqualVelocityMoveMethod.h"

std::optional<SteeringHandle> EqualVelocityMoveMethod::GetSteeringHandle(float Ratio, KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate, float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget)
{
	SteeringHandle Result;
	Result.Linear = Target.Position - Character.Position;
	if (Result.Linear.Length() < TargetRadius)
	{
		return std::nullopt;
	}
	Result.Linear /= TimeToTarget;
	if (Result.Linear.Size() > MaxSpeed)
	{
		Result.Linear.Normalize();
		Result.Linear *= MaxSpeed;
	}
	Result.Angular = 0.0f;
	return Result;
}
