// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/KinematicController.h"
#include "Engine/Engine.h"
#include "Character/PlayerCharacter.h"
#include "ChaseMethod.h"
#include "RunMethod.h"
#include "MoveMethod.h"

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
	TimeToTarget = 0.25f;
	SlowRadius = 100.0f;
	this->Owner = Owner;
	MaxRotation = 10.f;
}

KinematicController::~KinematicController()
{
	if (!BehaviorQueue.IsEmpty())
	{
		Behaviors* TempBehavior = nullptr;
		while (BehaviorQueue.Dequeue(TempBehavior))
		{
			delete TempBehavior;
		}
	}
}

void KinematicController::MoveToLocation(FVector ToDestination)
{
	TargetData.Position = ToDestination;
	MoveMethod* TempMove = new MoveMethod();
	BehaviorQueue.Enqueue(TempMove);
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

	SteeringHandle CharacterSteering;
	CharacterSteering.Linear = FVector::ZeroVector;
	CharacterSteering.Angular = 0.0f;

	TQueue<Behaviors*> BackupQueue;
	while(!BehaviorQueue.IsEmpty())
	{
		std::optional<SteeringHandle> TempHandle;
		Behaviors* TempBehavior = nullptr;
		BehaviorQueue.Dequeue(TempBehavior);
		if (TempBehavior == nullptr)
			continue;
		TempHandle = TempBehavior->GetSteeringHandle(CharacterData, TargetData, MaxSpeed, MaxAcceleration, MaxRotation, MaxAngular, BoardRadius, SlowRadius, TimeToTarget);
		if (!TempHandle)
		{
			// 더이상 움직임이 필요없는 운동은 삭제
			delete TempBehavior;
			FDateTime Now = FDateTime::Now();
			UE_LOG(LogTemp, Warning, TEXT("도착 ! %s"), *Now.ToString());
			UE_LOG(LogTemp, Warning, TEXT("도착 ! %s"), *CharacterData.Position.ToString());
			StopMove();
			continue;
		}
		// 속도와 회전 속도를 가속도와 각가속도에 따라 업데이트
		CharacterSteering.Linear += TempHandle->Linear;
		CharacterSteering.Angular += TempHandle->Angular;
		// 아직 운동량이 남았으니 다시 큐에 넣어줄 준비를한다.
		BackupQueue.Enqueue(TempBehavior);
	}

	//큐에 백업된 것들을 다시 큐에 넣어준다.
	if (!BackupQueue.IsEmpty())
	{
		while (!BackupQueue.IsEmpty())
		{
			Behaviors* TempBehavior = nullptr;
			BackupQueue.Dequeue(TempBehavior);
			BehaviorQueue.Enqueue(TempBehavior);
		}
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
	CharacterData.Velocity = FVector::ZeroVector;
	CharacterData.Rotation = 0;
	BehaviorQueue.Empty();
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





