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
	FVector Position; // 발사체의 위치
	FVector TargetPosition; // 목표 지점의 위치
	bool Arrived; // 목표 지점에 도착했는지 여부
	float Radius; // 목표 지점에 도착했다고 판단할 반지름
	FVector Velocity; // 발사체의 속도
	FVector Gravity; // 중력 가속도
public:
	ArcKinematicController(class AActor* Owner, FVector Start, FVector End, float Speed, FVector Gravity);
	void Update(float DeltaTime);
	bool CheckCanShoot();
	std::optional<FVector> CalculateVelocity(FVector Start, FVector End, float MuzzleV);
	~ArcKinematicController() = default;
};
