// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <optional>

/**
 * 
 */


struct SteeringHandle
{
	FVector Linear; // 가속도
	float Angular; // 각가속도
};

struct KinematicStatic
{
	FVector Position; // 위치
	float Orientation; // 방향
	FVector Velocity; // 속도
	float Rotation; // 회전
};

class Behaviors
{
public:
	virtual std::optional<SteeringHandle> GetSteeringHandle(KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate,
		float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget) = 0;
	virtual ~Behaviors() = default; // 가상 소멸자 추가
};

class PROJECTKJCLIENT_API KinematicController
{
private:
	class AActor* Owner;
	float MaxSpeed; // 최대 속도
	float MaxAngular; // 최대 각속도
	float BoardRadius; // 반경
	float MaxAcceleration; // 최대 가속도
	float MaxRotation; // 최대 회전
	float SlowRadius; // 감속 반경
	float TimeToTarget; // 목표까지 도달하는 시간
	KinematicStatic CharacterData;
	KinematicStatic TargetData;
	TQueue<Behaviors*,EQueueMode::Mpsc> BehaviorQueue;

	void SetRotation(float NewOrientation);
	void SetPosition(FVector NewPosition);
	float NewOrientation(float CurrentOrientation, FVector Velocity);

	void MakePlayMovingAnimation();
	void StopMovingAnimation();
	bool IsMovingNow();

public:
	KinematicController(class AActor* Owner, FVector Position, float MaxSpeed, float Radius, float MaxAccelerate);
	~KinematicController();
	void MoveToLocation(FVector Destination);
	void Update(float DeltaTime);
	void StopMove();
};
