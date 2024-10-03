// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/VelocityMatchMethod.h"

std::optional<SteeringHandle> VelocityMatchMethod::GetSteeringHandle(float Ratio, KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate, float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget)
{
	SteeringHandle Result;
	Result.Linear = Target.Velocity - Character.Velocity;
	Result.Linear /= TimeToTarget;
	if (Result.Linear.Size() > MaxAccelerate)
	{
		Result.Linear.Normalize();
		Result.Linear *= MaxAccelerate;
	}
	Result.Angular = 0.0f;
	return Result;
}
