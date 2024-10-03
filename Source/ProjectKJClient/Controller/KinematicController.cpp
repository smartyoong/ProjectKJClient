// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/KinematicController.h"
#include "Engine/Engine.h"
#include "Character/PlayerCharacter.h"
#include "ChaseMethod.h"
#include "RunMethod.h"
#include "MoveMethod.h"
#include "BrakeMethod.h"

const FVector MinusOneVector(-1.0f, -1.0f, -1.0f);

KinematicController::KinematicController(AActor* Owner ,FVector Position, float MaxSpeed, float Radius, float MaxAccelerate)
{
	MaxAngular = 1.f;
	this->MaxSpeed = MaxSpeed;
	BoardRadius = Radius;
	MaxAcceleration = MaxAccelerate;
	CharacterData.Position = Position;
	CharacterData.Orientation = 0.0f;
	CharacterData.Velocity = FVector::ZeroVector;
	CharacterData.Rotation = 0.0f;
	TargetData.Orientation = 0.0f;
	TargetData.Position = MinusOneVector;
	TargetData.Velocity = FVector::ZeroVector;
	TargetData.Rotation = 0.0f;
	TimeToTarget = 0.1f;
	SlowRadius = 100.0f;
	this->Owner = Owner;
	MaxRotation = 10.f;
}


void KinematicController::MoveToLocation(FVector ToDestination)
{
	TargetData.Position = ToDestination;
	AddMoveFlag(MoveType::Move);
}


void KinematicController::SetRotation(float NewOrientation) // 라디안
{
	FQuat QuatRotation = FQuat(FVector::UpVector, NewOrientation);
	if (Owner)
		Owner->SetActorRotation(QuatRotation);
}

void KinematicController::SetPosition(FVector NewPosition)
{
	if (Owner)
	{
		FVector CurrentPosition = Owner->GetActorLocation();
		FVector MixPosition{ NewPosition.X, NewPosition.Y, CurrentPosition.Z };
		Owner->SetActorLocation(MixPosition);
	}
}

void KinematicController::Update(float DeltaTime)
{
	// 위치와 방향을 속도에 따라 업데이트 (Z축은 사용하지 않음)
	CharacterData.Position.X += CharacterData.Velocity.X * DeltaTime;
	CharacterData.Position.Y += CharacterData.Velocity.Y * DeltaTime;
	CharacterData.Position.Z = 0.0f;
	CharacterData.Velocity.Z = 0.0f;
	TargetData.Position.Z = 0.0f;
	TargetData.Velocity.Z = 0.0f;

	CharacterData.Orientation += CharacterData.Rotation * DeltaTime;

	SetPosition(CharacterData.Position);
	SetRotation(CharacterData.Orientation);

	SteeringHandle CharacterSteering{FVector::ZeroVector,0};


	// 우선 순위 기반 행동 조합이다.

	if (HasMoveFlag(MoveType::Move))
	{
		MoveMethod Move;
		auto Result = Move.GetSteeringHandle(1, CharacterData, TargetData, MaxSpeed, MaxAcceleration, MaxRotation, MaxAngular, BoardRadius, SlowRadius, TimeToTarget);
		if (Result)
		{
			CharacterSteering += *Result;
		}
		else
		{
			RemoveMoveFlag(MoveType::Move);
			AddMoveFlag(MoveType::Brake);
		}
	}

	if (HasMoveFlag(MoveType::Chase))
	{
		ChaseMethod Chase;
		auto Result = Chase.GetSteeringHandle(1, CharacterData, TargetData, MaxSpeed, MaxAcceleration, MaxRotation, MaxAngular, BoardRadius, SlowRadius, TimeToTarget);
		if (Result)
		{
			CharacterSteering += *Result;
		}
		else
		{
			RemoveMoveFlag(MoveType::Chase);
		}
	}

	if (HasMoveFlag(MoveType::RunAway))
	{
		RunMethod RunAway;
		auto Result = RunAway.GetSteeringHandle(1, CharacterData, TargetData, MaxSpeed, MaxAcceleration, MaxRotation, MaxAngular, BoardRadius, SlowRadius, TimeToTarget);
		if (Result)
		{
			CharacterSteering += *Result;
		}
		else
		{
			RemoveMoveFlag(MoveType::RunAway);
		}
	}

	if (HasMoveFlag(MoveType::Brake))
	{
		BrakeMethod Brake;
		auto Result = Brake.GetSteeringHandle(1, CharacterData, TargetData, MaxSpeed, MaxAcceleration, MaxRotation, MaxAngular, BoardRadius, SlowRadius, DeltaTime);
		if (Result)
		{
			CharacterSteering += *Result;
		}
		else
		{
			RemoveMoveFlag(MoveType::Brake);
			AddMoveFlag(MoveType::VelocityStop);
			AddMoveFlag(MoveType::ForceAdjustPosition);
		}
	}

	// 속력을 완전 0으로 만들어주는 플래그 (강제 멈춤)
	if (HasMoveFlag(MoveType::VelocityStop))
	{
		CharacterData.Velocity = FVector::ZeroVector;
		CharacterSteering.Linear = FVector::ZeroVector;
		RemoveMoveFlag(MoveType::VelocityStop);
		UE_LOG(LogTemp, Warning, TEXT("Velocity Stop"));
	}
	// 위치를 강제로 조정하는 플래그
	if (HasMoveFlag(MoveType::ForceAdjustPosition))
	{
		CharacterData.Position = TargetData.Position;
		RemoveMoveFlag(MoveType::ForceAdjustPosition);
		UE_LOG(LogTemp, Warning, TEXT("Force Adjust Position %s"), *CharacterData.Position.ToString());
	}


	// 속도와 회전 속도를 가속도와 각가속도에 따라 업데이트
	CharacterData.Velocity.X += CharacterSteering.Linear.X * DeltaTime;
	CharacterData.Velocity.Y += CharacterSteering.Linear.Y * DeltaTime;
	CharacterData.Rotation += CharacterSteering.Angular * DeltaTime;

	if (CharacterData.Velocity.Size() > MaxSpeed)
	{
		CharacterData.Velocity.Normalize();
		CharacterData.Velocity *= MaxSpeed;
	}

	if (CharacterData.Rotation > MaxAngular)
	{
		CharacterData.Rotation = MaxAngular;
	}
	// 움직임 여부에 따라서 애니메이션 처리
	if (!IsMovingNow())
	{
		StopMovingAnimation();
	}
	else
	{
		MakePlayMovingAnimation();
	}
}

void KinematicController::StopMove()
{
	// 이동을 멈추고, 애니메이션을 멈춥니다.
	RemoveMoveFlag(MoveType::Move);
	AddMoveFlag(MoveType::VelocityStop);
}

float KinematicController::NewOrientation(float CurrentOrientation, FVector Velocity)
{
	if (Velocity.Length() > 0)
	{
		return FMath::Atan2(Velocity.Y, Velocity.X);
	}

	return CurrentOrientation;
}

void KinematicController::MakePlayMovingAnimation()
{
	//현재는 임시로 Player로 캐스팅하지만, PlayerCharacter 보다 상위에다가 해당 virtual 메서드를 만들어보자
	if (Owner)
	{
		auto Player = Cast<APlayerCharacter>(Owner);
		// AnimBP에서 조건을 만족시킨다.
		if (Player)
		{
			Player->PlayMovingAnim(true);
		}
	}
}

void KinematicController::StopMovingAnimation()
{
	if (Owner)
	{
		auto Player = Cast<APlayerCharacter>(Owner);
		// AnimBP에서 조건을 만족시킨다.
		if (Player)
		{
			Player->PlayMovingAnim(false);
		}
	}
}

bool KinematicController::IsMovingNow()
{
	return FMath::Abs(CharacterData.Rotation) > 1.f || CharacterData.Velocity.Length() > 3.f;
}

void KinematicController::AddMoveFlag(MoveType Flag)
{
	int32 InitialValue, NewValue;
	do
	{
		InitialValue = MoveFlag.load();
		NewValue = InitialValue | static_cast<int32>(Flag);
	} while (!MoveFlag.compare_exchange_weak(InitialValue, NewValue));
}

void KinematicController::RemoveMoveFlag(MoveType Flag)
{
	int32 InitialValue, NewValue;
	do
	{
		InitialValue = MoveFlag.load();
		NewValue = InitialValue & ~static_cast<int32>(Flag);
	} while (!MoveFlag.compare_exchange_weak(InitialValue, NewValue));
}

bool KinematicController::HasMoveFlag(MoveType Flag)
{
	return (MoveFlag.load() & static_cast<int32>(Flag)) == static_cast<int32>(Flag);
}





