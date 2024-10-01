// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/ChaseMethod.h"
#include "Controller/KinematicController.h"


std::optional<SteeringHandle> ChaseMethod::GetSteeringHandle(KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate, float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget)
{
    SteeringHandle Result{ FVector::ZeroVector, 0 };
    Result.Linear = Target.Position - Character.Position;
    Result.Linear.Normalize();
    Result.Linear *= MaxAccelerate;
    return Result;
}
