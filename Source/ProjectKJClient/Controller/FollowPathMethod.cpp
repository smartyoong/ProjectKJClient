// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/FollowPathMethod.h"
#include "Controller/EqualVelocityChaseMethod.h"

FollowPathMethod::FollowPathMethod(PathComponent* PathComponent) : Path(PathComponent)
{
}

std::optional<SteeringHandle> FollowPathMethod::GetSteeringHandle(float Ratio, KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate, float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget)
{
    KinematicStatic TargetPoint = Target;
    int CurrentIndex = Path->GetCurrentIndex();
    TargetPoint.Position = Path->GetPosition(CurrentIndex);

    if (Path->Arrived(Character.Position))
    {
        CurrentIndex = Path->GetNextPositionIndex();
        TargetPoint.Position = Path->GetPosition(CurrentIndex);
    }

    EqualVelocityChaseMethod Chase;
    return Chase.GetSteeringHandle(Ratio, Character, TargetPoint, MaxSpeed, MaxAccelerate, MaxRotation, MaxAngular, TargetRadius, SlowRadius, TimeToTarget);
}
