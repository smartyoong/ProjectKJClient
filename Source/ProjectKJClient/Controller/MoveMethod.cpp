// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MoveMethod.h"

std::optional<SteeringHandle> MoveMethod::GetSteeringHandle(float Ratio, KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate, float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget)
{
	SteeringHandle Result{ FVector::ZeroVector, 0 };

	FVector Direction = Target.Position - Character.Position;
	float Distance = Direction.Length();

	// 현재 속도의 제곱 크기
	//float CurrentSpeedSqr = Character.Velocity.SquaredLength();

	// 브레이크 거리를 계산하여 TargetRadius를 동적으로 설정
	//float DynamicTargetRadius = CurrentSpeedSqr / (2 * MaxAccelerate);

	if (Distance < TargetRadius)
	{
		return std::nullopt;
	}

	float TargetSpeed = MaxSpeed;
	//float DynamicSlowRadius = DynamicTargetRadius * 2; // 브레이크 지점에 비해서 넒은 지점을 감속 지점으로 지정
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
	Result.Linear *= Ratio;
	Result.Angular = 0;
	return Result;
}
