// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BrakeMethod.h"

std::optional<SteeringHandle> BrakeMethod::GetSteeringHandle(float Ratio, KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate, float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget)
{
    // 현재 속도 방향의 반대 방향으로 브레이크 가속도 적용
    FVector CurrentVelocity = Character.Velocity;
    CurrentVelocity.Normalize();
    FVector BrakeDirection = -CurrentVelocity;
    FVector BrakeForce = BrakeDirection * MaxAccelerate;

    // 속도가 매우 작아졌을 때 객체를 완전히 멈추도록 설정
    const float VelocityThreshold = 0.01f; // 속도가 매우 작다고 간주할 임계값

    // 속도가 0을 넘어 반대 방향으로 가는 것을 방지 (null을 받으면 정지)
    float Diff = Character.Velocity.Length() - (BrakeForce.Length() * TimeToTarget); // 여기서 TimeToTarget은 DeltaTime
    if (Diff < VelocityThreshold || Character.Velocity.Dot(BrakeDirection) > 0)
    {
        return std::nullopt;
    }
    BrakeForce *= Ratio;
    return SteeringHandle{ BrakeForce, 0 };
}
