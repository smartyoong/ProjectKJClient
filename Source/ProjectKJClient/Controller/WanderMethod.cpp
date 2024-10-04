// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/WanderMethod.h"
#include "WanderMethod.h"
#include "LockOnMethod.h"

std::optional<SteeringHandle> WanderMethod::GetSteeringHandle(float Ratio, KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate, float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget)
{

	// 배회하는 유닛의 offset 만큼 앞에 radius 만큼의 원이 존재하고,
	// 그 원 위에서 랜덤한 방향으로 최대 Rate 회전만큼 회전하고 움직입니다.

	float WanderOffset = 300.f;
	float WanderRadius = 50.f;
	float WanderRate = 30.f;
	float WanderOrientation = 0.f;


	WanderOrientation += (FMath::FRandRange(0.f, 1.0f) - FMath::FRandRange(0.f, 1.0f)) * WanderRate;
	float TargetOrientation = WanderOrientation + Character.Orientation;
	FVector TargetPosition = Character.Position + WanderOffset * FVector(FMath::Cos(Character.Orientation), FMath::Sin(Character.Orientation), 0.f);
	TargetPosition += WanderRadius * FVector(FMath::Cos(TargetOrientation), FMath::Sin(TargetOrientation), 0.f);
	auto Angular = LockOnMethod().GetSteeringHandle(Ratio, Character, { TargetPosition, TargetOrientation, FVector::ZeroVector, 0.f }, 
		MaxSpeed, MaxAccelerate, MaxRotation, MaxAngular, TargetRadius, SlowRadius, TimeToTarget);

	SteeringHandle Result;
	Result.Linear = FVector(FMath::Cos(Character.Orientation), FMath::Sin(Character.Orientation), 0.f) * MaxAccelerate;
	if (Angular)
	{
		Result.Angular = Angular->Angular;
	}
	else
	{
		Result.Angular = 0.f;
	}
	return Result;
}
