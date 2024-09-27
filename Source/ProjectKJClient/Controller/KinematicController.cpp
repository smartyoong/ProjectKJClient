// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/KinematicController.h"
#include "Engine/Engine.h"
#include "Character/PlayerCharacter.h"

const FVector MinusOneVector(-1.0f, -1.0f, -1.0f);

KinematicController::KinematicController(AActor* Owner ,FVector Position, float MaxSpeed, float Radius)
{
	Limit.MaxAngular = 1.f;
	Limit.MaxSpeed = MaxSpeed;
	Limit.Radius = Radius;
	Handle.Velocity = FVector::ZeroVector;
	Handle.Rotation = 0.0f;
	Steering.Linear = FVector::ZeroVector;
	Steering.Angular = 0.0f;
	Destination = MinusOneVector;
	this->Owner = Owner;
}

KinematicController::~KinematicController()
{
}

void KinematicController::MoveToLocation(FVector ToDestination)
{
	// 이동 실패여부를 따지지 않고 이동시킵니다.
	KinematicHandle NewHandle;
	// 목적지까지의 가속도 및 방향을 계산합니다.
	NewHandle.Velocity = ToDestination - GetCurrentPosition();
	NewHandle.Velocity.Normalize();
	NewHandle.Velocity *= Limit.MaxSpeed;
	NewHandle.Rotation = 0;
	// 이동을 위한 핸들을 업데이트합니다.
	FScopeLock Lock(&CS);
	SetDestination(ToDestination);
	SetKinematicHandle(NewHandle);
	SetRotation(NewOrientation(GetCurrentOrientation(), Handle.Velocity));
	MakePlayMovingAnimation();
}

void KinematicController::RunFromTarget(FVector TargetPosition)
{
	// 이동 실패여부를 따지지 않고 이동시킵니다.
	// 종료 조건은 존재하지 않습니다. 다른 방법으로 핸들을 조정해야합니다.
	KinematicHandle NewHandle;
	// 목적지까지의 가속도 및 방향을 계산합니다.
	NewHandle.Velocity = GetCurrentPosition() - TargetPosition;
	NewHandle.Velocity.Normalize();
	NewHandle.Velocity *= Limit.MaxSpeed;
	NewHandle.Rotation = 0;
	// 이동을 위한 핸들을 업데이트합니다.
	SetKinematicHandle(NewHandle);
	SetRotation(NewOrientation(GetCurrentOrientation(), Handle.Velocity));
	MakePlayMovingAnimation();
}

void KinematicController::Wandering()
{
	// 이동 실패여부를 따지지 않고 이동시킵니다.
	// 종료 조건은 존재하지 않습니다. 다른 방법으로 핸들을 조정해야합니다.
	KinematicHandle NewHandle;
	NewHandle.Velocity = Limit.MaxSpeed * FVector(FMath::Cos(GetCurrentOrientation()), FMath::Sin(GetCurrentOrientation()), 0);
	NewHandle.Rotation = (FMath::FRand()-FMath::FRand()) * Limit.MaxAngular;
	// 이동을 위한 핸들을 업데이트합니다.
	SetKinematicHandle(NewHandle);
	MakePlayMovingAnimation();
}

float KinematicController::GetCurrentOrientation()
{
	if (Owner)
		return FMath::DegreesToRadians(Owner->GetActorRotation().Yaw);
	return -1.f;
}

FVector KinematicController::GetCurrentPosition()
{
	if (Owner)
		return Owner->GetActorLocation();
	return MinusOneVector;
}

FVector KinematicController::GetDestination()
{
	return Destination;
}

void KinematicController::SetDestination(FVector ToDestination)
{
	FScopeLock Lock(&CS);
	this->Destination = ToDestination;
}

void KinematicController::SetKinematicHandle(KinematicHandle NewHandle)
{
	FScopeLock Lock(&CS);
	Handle = NewHandle;
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
		Owner->SetActorLocation(NewPosition);
}

void KinematicController::Update(float DeltaTime)
{
	// 위치와 방향을 속도에 따라 업데이트
	SetPosition(GetCurrentPosition() + Handle.Velocity * DeltaTime);
	SetRotation(GetCurrentOrientation() + Handle.Rotation * DeltaTime);

	// 목적지가 설정되어있으면, 목적지에 도착했는지 확인
	if (Destination != MinusOneVector)
	{
		Arrive();
	}

	// 속도와 회전 속도를 가속도와 각가속도에 따라 업데이트
	FScopeLock Lock(&CS);
	Handle.Velocity += Steering.Linear * DeltaTime;
	Handle.Rotation += Steering.Angular * DeltaTime;
}

void KinematicController::StopMove()
{
	SetDestination(MinusOneVector);
	StopMovingAnimation();
	KinematicHandle NewHandle;
	NewHandle.Velocity = FVector::ZeroVector;
	NewHandle.Rotation = 0;
	SetKinematicHandle(NewHandle);
}

float KinematicController::NewOrientation(float CurrentOrientation, FVector Velocity)
{
	if (Velocity.Size() > 0)
	{
		return FMath::Atan2(Velocity.Y, Velocity.X);
	}

	return CurrentOrientation;
}

void KinematicController::Arrive()
{
	KinematicHandle NewHandle;
	FVector CurrentPosition = GetCurrentPosition();
	FVector CurrentDestination = GetDestination();
	NewHandle.Velocity = CurrentDestination - CurrentPosition;
	// 목적지에 도착했는지 확인
	if (NewHandle.Velocity.Length() < Limit.Radius)
	{
		NewHandle.Velocity = FVector::ZeroVector;
		NewHandle.Rotation = 0;
		SetPosition(CurrentDestination);
		SetDestination(MinusOneVector);
		StopMovingAnimation();
	}
	else
	{
		const float TimeToTarget = 0.25f;
		NewHandle.Velocity /= TimeToTarget;
		if (NewHandle.Velocity.Length() > Limit.MaxSpeed)
		{
			NewHandle.Velocity.Normalize();
			NewHandle.Velocity *= Limit.MaxSpeed;
			NewHandle.Rotation = 0;
		}
	}
	SetKinematicHandle(NewHandle);
	SetRotation(NewOrientation(GetCurrentOrientation(), Handle.Velocity));
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





