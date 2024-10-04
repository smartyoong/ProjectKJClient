// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PursueMethod.h"
#include "ChaseMethod.h"

std::optional<SteeringHandle> PursueMethod::GetSteeringHandle(float Ratio, KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate, float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget)
{
	const float MaxPrediction = 30.0f;
	FVector Direction = Target.Position - Character.Position;
	float Distance = Direction.Size();
	float Speed = Character.Velocity.Size();
	float Prediction = 0.0f;
	if (Speed <= Distance / MaxPrediction)
	{
		Prediction = MaxPrediction;
	}
	else
	{
		Prediction = Distance / Speed;
	}
	KinematicStatic NewTarget = Target;
	NewTarget.Position += NewTarget.Velocity * Prediction;
	return ChaseMethod().GetSteeringHandle(Ratio, Character, NewTarget, MaxSpeed, MaxAccelerate, MaxRotation, MaxAngular, TargetRadius, SlowRadius, TimeToTarget);
}
