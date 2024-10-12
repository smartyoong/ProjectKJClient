// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <optional>
#include <atomic>
#include "PathComponent.h"

/**
 * 
 */


struct SteeringHandle
{
	FVector Linear; // 가속도
	float Angular = 0.f; // 각가속도
	SteeringHandle& operator += (const SteeringHandle& Other)
	{
		Linear += Other.Linear;
		Angular += Other.Angular;
		return *this;
	}
};

struct KinematicStatic
{
	FVector Position; // 위치
	float Orientation = 0.f; // 방향
	FVector Velocity; // 속도
	float Rotation =0.f; // 회전
};

enum class MoveType : int32
{
	None = 0,
	Move = 1 << 0,
	VelocityStop = 1 << 1,
	EqualVelocityMove = 1 << 2,
	RotateStop = 1 << 3,
	LockOn = 1 << 4,
	LookAtToMove = 1 << 5,
	EqualVelocityWander = 1 << 6,
	FollowPath = 1 << 7,
	EqualVelocityChase = 1 << 8,
	EqualVelocityRunAway = 1 << 9,
	OrientationChange = 1 << 10,
};
// 비트 연산을 지원하기 위한 매크로
ENUM_CLASS_FLAGS(MoveType)

class Behaviors
{
public:
	virtual std::optional<SteeringHandle> GetSteeringHandle(float Ratio, KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate,
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
	std::atomic<int32>MoveFlag;

	PathComponent* PathComp;

	void SetRotation(float NewOrientation);
	void SetPosition(FVector NewPosition);
	float NewOrientation(float CurrentOrientation, FVector Velocity);

	void MakePlayMovingAnimation();
	void StopMovingAnimation();
	bool IsMovingNow();
	void AddMoveFlag(MoveType Flag);
	void RemoveMoveFlag(MoveType Flag);
	bool HasMoveFlag(MoveType Flag);

public:
	KinematicController(class AActor* Owner, FVector Position, float MaxSpeed, float Radius, float MaxAccelerate, PathComponent* Path);
	~KinematicController() = default;
	void MoveToLocation(FVector Destination);
	void Update(float DeltaTime);
	void StopMove(FVector* Target);

};
