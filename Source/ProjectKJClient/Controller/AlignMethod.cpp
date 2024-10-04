// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/AlignMethod.h"

std::optional<SteeringHandle> AlignMethod::GetSteeringHandle(float Ratio, KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate, float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget)
{
    SteeringHandle Result{ FVector::ZeroVector, 0 };
	float Rotation = FMath::FindDeltaAngleRadians(Character.Orientation,Target.Orientation);
    float RotationSize = FMath::Abs(Rotation);


    //회전을 강제로 멈추도록 요청한다.
    if (RotationSize < 0.1f)
    {
        return std::nullopt;
    }

    float GoalRotation = 0;
    if (RotationSize > 5.f)
    {
        GoalRotation = MaxRotation;
    }
    else
    {
        GoalRotation = (MaxRotation * RotationSize) / 5.f;
    }

    GoalRotation *= (Rotation / RotationSize);


    Result.Angular = GoalRotation - Character.Rotation;
    Result.Angular /= TimeToTarget;

    float AngularAcceleration = FMath::Abs(Result.Angular);
    if (AngularAcceleration > MaxAngular)
    {
        Result.Angular /= AngularAcceleration;
        Result.Angular *= MaxAngular;
    }

    Result.Linear = FVector::ZeroVector;
    return Result;
}
