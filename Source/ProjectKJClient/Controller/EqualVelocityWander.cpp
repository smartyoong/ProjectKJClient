// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/EqualVelocityWander.h"
#include "EqualVelocityWander.h"

std::optional<SteeringHandle> EqualVelocityWanderMethod::GetSteeringHandle(float Ratio, KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate, float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget)
{
	SteeringHandle Result {FVector::ZeroVector,0};
	// 랜덤한 방향을 생성합니다.
	float RandomOrientation = FMath::FRandRange(0.f, 1.0f) - FMath::FRandRange(0.f, 1.0f);
	float X = FMath::Cos(Character.Orientation);
	float Y = FMath::Sin(Character.Orientation);
	Result.Linear = MaxSpeed * FVector{ X,Y,0.0f };
	Result.Angular = RandomOrientation * MaxRotation;
	return Result;
}
