// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/EqualVelocityChaseMethod.h"

std::optional<SteeringHandle> EqualVelocityChaseMethod::GetSteeringHandle(float Ratio, KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate, float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget)
{
	SteeringHandle Result;
	Result.Linear = Target.Position - Character.Position;
	Result.Linear.Normalize();
	Result.Linear *= MaxSpeed;
	Result.Angular = 0.f;
	return Result;
}
