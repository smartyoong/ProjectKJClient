// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

struct StreeringHandle
{
	FVector Linear; // 가속도
	float Angular; // 각가속도
};

struct KinematicStatic
{
	FVector Position; // 위치
	float Orientation; // 방향
};

struct LimitData
{
	float MaxSpeed; // 최대 속도
	float MaxAngular; // 최대 각속도
	float Radius; // 반경
};

struct KinematicHandle
{
	FVector Velocity;
	float Rotation;
};

class PROJECTKJCLIENT_API KinematicController
{
private:
	class AActor* Owner;
	LimitData Limit;
	KinematicHandle Handle;
	StreeringHandle Steering;
	FVector Destination;
	FCriticalSection CS;
	float GetCurrentOrientation();
	FVector GetCurrentPosition();
	FVector GetDestination();
	void SetDestination(FVector Destination);
	void SetKinematicHandle(KinematicHandle NewHandle);
	void SetRotation(float NewOrientation);
	void SetPosition(FVector NewPosition);
	float NewOrientation(float CurrentOrientation, FVector Velocity);
	void Arrive();
	void MakePlayMovingAnimation();
	void StopMovingAnimation();
public:
	KinematicController(class AActor* Owner, FVector Position, float MaxSpeed, float Radius);
	~KinematicController();
	void MoveToLocation(FVector Destination);
	void RunFromTarget(FVector TargetPosition);
	void Wandering();
	void Update(float DeltaTime);
	void StopMove();
};
