// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <optional>

/**
 * 
 */
class PROJECTKJCLIENT_API ArcKinematicController
{
private:
	class AActor* Owner;
	float Speed; // 발사체의 속도(스칼라)
	FVector StartPosition; // 발사체의 시작 위치
	FVector Position; // 발사체의 위치
	FVector TargetPosition; // 목표 지점의 위치
	bool IsLaunched; // 목표 지점에 도착했는지 여부
	FVector Velocity; // 발사체의 속도
	FVector Gravity; // 중력 가속도
	float ElapsedTime; // 경과 시간
public:
	ArcKinematicController(class AActor* Owner, FVector Start, FVector End, float Speed);
	void Update(float DeltaTime);
	~ArcKinematicController() = default;
	void LaunchAnimation();
	void StopLaunchAnimation();
	bool CalculateTrajectory();
	float CalculateLaunchAngle(float HorizontalDistance, float Height, float Speed, float Gravity);
	float CalculateInitialSpeed(FVector Start, FVector Target, float Angle, FVector Gravity);
	void Launch(float Radian);
};
