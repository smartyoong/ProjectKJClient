// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/LockOnMethod.h"
#include "AlignMethod.h"

std::optional<SteeringHandle> LockOnMethod::GetSteeringHandle(float Ratio, KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate, float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget)
{
	FVector Direction = Target.Position - Character.Position;
	if (Direction.Size() == 0)
	{
		return std::nullopt;
	}
	KinematicStatic NewTarget = Target;
	NewTarget.Orientation = FMath::Atan2(Direction.Y, Direction.X);
	return AlignMethod().GetSteeringHandle(Ratio, Character, NewTarget, MaxSpeed, MaxAccelerate, MaxRotation, MaxAngular, TargetRadius, SlowRadius, TimeToTarget);
}
