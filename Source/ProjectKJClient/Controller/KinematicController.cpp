// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/KinematicController.h"
#include "Engine/Engine.h"
#include "Character/PlayerCharacter.h"
#include "ChaseMethod.h"
#include "RunMethod.h"
#include "MoveMethod.h"
#include "BrakeMethod.h"
#include "AlignMethod.h"
#include "EqualVelocityMoveMethod.h"
#include "VelocityMatchMethod.h"
#include "PursueMethod.h"
#include "LockOnMethod.h"
#include "LookAtToMoveMethod.h"
#include "EqualVelocityWander.h"
#include "WanderMethod.h"
#include "PathComponent.h"
#include "FollowPathMethod.h"
#include "SeperateMethod.h"
#include "CollisionAvoidanceMethod.h"
#include "ObstacleAvoidanceMethod.h"
#include "EqualVelocityChaseMethod.h"
#include "EqualVelocityRunAwayMethod.h"

const FVector MinusOneVector(-1.0f, -1.0f, -1.0f);

KinematicController::KinematicController(AActor* Owner ,FVector Position, float MaxSpeed, float Radius, float MaxAccelerate, PathComponent* Path)
{
	MaxAngular = 30.f;
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
	MaxRotation = 100.f;
	PathComp = Path;
}


void KinematicController::MoveToLocation(FVector ToDestination)
{
	TargetData.Position = ToDestination;
	TargetData.Position.Z = 0.0f;
	CharacterData.Position.Z = 0.0f;
	AddMoveFlag(MoveType::EqualVelocityMove);
	//이거 되는지 테스트 해봅시다. 되긴하는데 체감이 안좋다 빙판 걷는 느낌이랄까?
	//AddMoveFlag(MoveType::Move);
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
		FHitResult HitResult;
		Owner->SetActorLocation(MixPosition, true, &HitResult);

		// 충돌이 발생했는지 확인
		if (HitResult.IsValidBlockingHit())
		{
			UE_LOG(LogTemp, Warning, TEXT("Collision detected with: %s"), *HitResult.GetActor()->GetName());
			StopMove();
		}
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
			AddMoveFlag(MoveType::VelocityStop);
		}
	}

	if (HasMoveFlag(MoveType::Pursue))
	{
		PursueMethod Pursue;
		auto Result = Pursue.GetSteeringHandle(1, CharacterData, TargetData, MaxSpeed, MaxAcceleration, MaxRotation, MaxAngular, BoardRadius, SlowRadius, TimeToTarget);
		if (Result)
		{
			CharacterSteering += *Result;
		}
		else
		{
			RemoveMoveFlag(MoveType::Pursue);
		}
	}

	if (HasMoveFlag(MoveType::VelocityMatch))
	{
		VelocityMatchMethod VelocityMatch;
		auto Result = VelocityMatch.GetSteeringHandle(1, CharacterData, TargetData, MaxSpeed, MaxAcceleration, MaxRotation, MaxAngular, BoardRadius, SlowRadius, TimeToTarget);
		if (Result)
		{
			CharacterSteering += *Result;
		}
		else
		{
			RemoveMoveFlag(MoveType::VelocityMatch);
		}
	}

	if (HasMoveFlag(MoveType::Chase))
	{
		ChaseMethod Chase;
		auto Result = Chase.GetSteeringHandle(1, CharacterData, TargetData, MaxSpeed, MaxAcceleration, MaxRotation, MaxAngular, BoardRadius, SlowRadius, TimeToTarget);
		if (Result)
		{
			CharacterSteering += *Result;
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
		}
	}

	if (HasMoveFlag(MoveType::Wander))
	{
		WanderMethod Wander;
		auto Result = Wander.GetSteeringHandle(1, CharacterData, TargetData, MaxSpeed, MaxAcceleration, MaxRotation, MaxAngular, BoardRadius, SlowRadius, TimeToTarget);
		if (Result)
		{
			// 가속도를 1번만 추가하니까 늦을수도 있음 (어울릴지도?)
			CharacterSteering += *Result;
			RemoveMoveFlag(MoveType::Wander);
		}
	}

	if (HasMoveFlag(MoveType::FollowPath))
	{
		if (PathComp == nullptr)
		{
			RemoveMoveFlag(MoveType::FollowPath);
			return;
		}

		FollowPathMethod FollowPath(PathComp);
		auto Result = FollowPath.GetSteeringHandle(1, CharacterData, TargetData, MaxSpeed, MaxAcceleration, MaxRotation, MaxAngular, BoardRadius, SlowRadius, TimeToTarget);
		if (Result)
		{
			CharacterSteering += *Result;
		}
	}

	if (HasMoveFlag(MoveType::Seperate))
	{
		//실제로 사용할 때는 타겟을 넣어주어야 한다.
		SeperateMethod Seperate(nullptr);
		auto Result = Seperate.GetSteeringHandle(1, CharacterData, TargetData, MaxSpeed, MaxAcceleration, MaxRotation, MaxAngular, BoardRadius, SlowRadius, TimeToTarget);
		if (Result)
		{
			CharacterSteering += *Result;
		}
	}

	if (HasMoveFlag(MoveType::CollisionAvoidance))
	{
		//실제로 사용할 때는 타겟을 넣어주어야 한다.
		CollisionAvoidanceMethod CollisionAvoidance(nullptr);
		//여기서 BoardRadius 대신 Collision Component의 Radius를 넣어주어야 한다.
		auto Result = CollisionAvoidance.GetSteeringHandle(1, CharacterData, TargetData, MaxSpeed, MaxAcceleration, MaxRotation, MaxAngular, BoardRadius, SlowRadius, TimeToTarget);
		if (Result)
		{
			CharacterSteering += *Result;
		}
	}

	if (HasMoveFlag(MoveType::ObstacleAvoidance))
	{
		// 아직 서버가 구현되지 않았다.
		ObstacleAvoidanceMethod ObstacleAvoidance(Owner);
		auto Result = ObstacleAvoidance.GetSteeringHandle(1, CharacterData, TargetData, MaxSpeed, MaxAcceleration, MaxRotation, MaxAngular, BoardRadius, SlowRadius, TimeToTarget);
		if (Result)
		{
			CharacterSteering += *Result;
		}
		else
		{
			RemoveMoveFlag(MoveType::ObstacleAvoidance);
		}
	}


	if (HasMoveFlag(MoveType::EqualVelocityMove))
	{
		EqualVelocityMoveMethod EqualVelocityMove;
		auto Result = EqualVelocityMove.GetSteeringHandle(1, CharacterData, TargetData, MaxSpeed, MaxAcceleration, MaxRotation, MaxAngular, BoardRadius, SlowRadius, TimeToTarget);
		if (Result)
		{
			// 이 메서드는 등속도 운동이다.
			CharacterData.Velocity += Result->Linear;
			AddMoveFlag(MoveType::OrientationChange);
		}
		else
		{
			RemoveMoveFlag(MoveType::EqualVelocityMove);
			AddMoveFlag(MoveType::VelocityStop);
		}
	}

	if (HasMoveFlag(MoveType::EqualVelocityWander))
	{
		EqualVelocityWanderMethod EqualVelocityWander;
		auto Result = EqualVelocityWander.GetSteeringHandle(1, CharacterData, TargetData, MaxSpeed, MaxAcceleration, MaxRotation, MaxAngular, BoardRadius, SlowRadius, TimeToTarget);
		if (Result)
		{
			CharacterData.Velocity += Result->Linear;
			CharacterData.Rotation += Result->Angular;
			//한번 배회하면 일단 다시 배회는 다음에 하자
			RemoveMoveFlag(MoveType::EqualVelocityWander);
			AddMoveFlag(MoveType::OrientationChange);
		}
	}

	if (HasMoveFlag(MoveType::EqualVelocityChase))
	{
		EqualVelocityChaseMethod EqualVelocityChase;
		auto Result = EqualVelocityChase.GetSteeringHandle(1, CharacterData, TargetData, MaxSpeed, MaxAcceleration, MaxRotation, MaxAngular, BoardRadius, SlowRadius, TimeToTarget);
		if (Result)
		{
			CharacterData.Velocity += Result->Linear;
			CharacterData.Rotation += Result->Angular;
			RemoveMoveFlag(MoveType::EqualVelocityChase);
			AddMoveFlag(MoveType::OrientationChange);
		}
	}

	if (HasMoveFlag(MoveType::EqualVelocityRunAway))
	{
		EqualVelocityRunAwayMethod EqualVelocityRunAway;
		auto Result = EqualVelocityRunAway.GetSteeringHandle(1, CharacterData, TargetData, MaxSpeed, MaxAcceleration, MaxRotation, MaxAngular, BoardRadius, SlowRadius, TimeToTarget);
		if (Result)
		{
			CharacterData.Velocity += Result->Linear;
			CharacterData.Rotation += Result->Angular;
			RemoveMoveFlag(MoveType::EqualVelocityRunAway);
			AddMoveFlag(MoveType::OrientationChange);
		}
	}

	// 속력을 완전 0으로 만들어주는 플래그 (강제 멈춤, 위치 강제 조정)
	if (HasMoveFlag(MoveType::VelocityStop))
	{
		CharacterData.Velocity = FVector::ZeroVector;
		CharacterSteering.Linear = FVector::ZeroVector;
		CharacterData.Position = TargetData.Position;
		RemoveMoveFlag(MoveType::VelocityStop);
	}


	// 속도와 회전 속도를 가속도와 각가속도에 따라 업데이트
	CharacterData.Velocity.X += (CharacterSteering.Linear.X * DeltaTime);
	CharacterData.Velocity.Y += (CharacterSteering.Linear.Y * DeltaTime);

	if (CharacterData.Velocity.Size() > MaxSpeed)
	{
		CharacterData.Velocity.Normalize();
		CharacterData.Velocity *= MaxSpeed;
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

	if (HasMoveFlag(MoveType::LookAtToMove))
	{
		LookAtToMoveMethod LookAtToMove;
		auto Result = LookAtToMove.GetSteeringHandle(1, CharacterData, TargetData, MaxSpeed, MaxAcceleration, MaxRotation, MaxAngular, BoardRadius, SlowRadius, TimeToTarget);
		if (Result)
		{
			CharacterSteering += *Result;
		}
		else
		{
			RemoveMoveFlag(MoveType::LookAtToMove);
			AddMoveFlag(MoveType::RotateStop);
		}
	}

	if (HasMoveFlag(MoveType::LockOn))
	{
		LockOnMethod LockOn;
		auto Result = LockOn.GetSteeringHandle(1, CharacterData, TargetData, MaxSpeed, MaxAcceleration, MaxRotation, MaxAngular, BoardRadius, SlowRadius, TimeToTarget);
		if (Result)
		{
			CharacterSteering += *Result;
		}
		else
		{
			RemoveMoveFlag(MoveType::LockOn);
			AddMoveFlag(MoveType::RotateStop);
		}
	}

	if (HasMoveFlag(MoveType::Align))
	{
		AlignMethod Align;
		auto Result = Align.GetSteeringHandle(1, CharacterData, TargetData, MaxSpeed, MaxAcceleration, MaxRotation, MaxAngular, 2.f, 10.f, TimeToTarget);
		if (Result)
		{
			CharacterSteering += *Result;
		}
		else
		{
			RemoveMoveFlag(MoveType::Align);
			AddMoveFlag(MoveType::RotateStop);
		}
	}

	CharacterData.Rotation += (CharacterSteering.Angular * DeltaTime);
	if (CharacterData.Rotation > MaxAngular)
	{
		CharacterData.Rotation = MaxAngular;
	}

	if (HasMoveFlag(MoveType::OrientationChange))
	{
		CharacterData.Orientation = NewOrientation(CharacterData.Orientation, TargetData.Position - CharacterData.Position);
		RemoveMoveFlag(MoveType::OrientationChange);
	}

	// 회전을 강제로 멈추는 플래그
	if (HasMoveFlag(MoveType::RotateStop))
	{
		CharacterData.Rotation = 0;
		CharacterSteering.Angular = 0;
		RemoveMoveFlag(MoveType::RotateStop);
	}
}

void KinematicController::StopMove()
{
	// 이동을 멈추고, 애니메이션을 멈춥니다.
	RemoveMoveFlag(MoveType::Move);
	RemoveMoveFlag(MoveType::Chase);
	RemoveMoveFlag(MoveType::RunAway);
	RemoveMoveFlag(MoveType::EqualVelocityMove);
	TargetData.Position = CharacterData.Position;
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





