// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/RunMethod.h"

std::optional<SteeringHandle> RunMethod::GetSteeringHandle(float Ratio, KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate, float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget)
{
    // 타겟의 반대방향으로 이동합니다.
    // 해당 타겟이 추적중일때 타겟의 반대방향으로 계속 도망칠때 사용합니다.

    SteeringHandle Result = {FVector::ZeroVector, 0};
    Result.Linear = Character.Position - Target.Position;
    Result.Linear.Normalize();
    Result.Linear *= MaxAccelerate;
    Result.Linear *= Ratio;
    return Result;
}
