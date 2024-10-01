// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MoveMethod.h"

std::optional<SteeringHandle> MoveMethod::GetSteeringHandle(KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate, float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget)
{
	SteeringHandle Result{ FVector::ZeroVector, 0 };

	FVector Direction = Target.Position - Character.Position;
	float Distance = Direction.Length();

	if (Distance < TargetRadius)
	{
		return std::nullopt;
	}

	float TargetSpeed = MaxSpeed;
	if (Distance < SlowRadius)
	{
		TargetSpeed = MaxSpeed * Distance / SlowRadius;
	}

	FVector TargetVelocity = Direction;
	TargetVelocity.Normalize();
	TargetVelocity *= TargetSpeed;

	Result.Linear = TargetVelocity - Character.Velocity;
	Result.Linear /= TimeToTarget;

	if (Result.Linear.Length() > MaxAccelerate)
	{
		Result.Linear.Normalize();
		Result.Linear *= MaxAccelerate;
	}

	Result.Angular = 0;
	return Result;
}
