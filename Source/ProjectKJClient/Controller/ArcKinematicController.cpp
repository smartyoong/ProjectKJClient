// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/ArcKinematicController.h"
#include "GameFramework/Actor.h"
#include "PlayerCharacter.h"


ArcKinematicController::ArcKinematicController(AActor* Owner, FVector Start, FVector End, float Speed)
{
	this->Owner = Owner;
	this->Position = Start;
	this->StartPosition = Start;
	this->TargetPosition = End;
	this->IsLaunched = false;
	this->Velocity = FVector::ZeroVector;
	this->Gravity = Owner->GetWorld()->GetGravityZ() * FVector::UpVector; // Z축 중력 가속도
	this->ElapsedTime = 0.0f;

	// 속도가 0보다 크면 속도를 설정하고, 아니면 초기 속도를 계산한다 가장 빠른 45도로.
	if (Speed > 0)
		this->Speed = Speed;
	else
		this->Speed = CalculateInitialSpeed(Start, End, 45.f, Gravity);
}

void ArcKinematicController::Update(float DeltaTime)
{
	if (!IsLaunched)
	{
		StopLaunchAnimation();
		if (Owner)
			Owner->SetActorLocation(TargetPosition); //보정
		return;
	}
	ElapsedTime += DeltaTime;

	// 액터가 존재한다면, Actor의 위치 업데이트
	if(Owner)
		Owner->SetActorLocation(Position);

	LaunchAnimation();

	FVector Displacement = Velocity * ElapsedTime + 0.5f * Gravity * ElapsedTime * ElapsedTime;
	Position = StartPosition + Displacement;
	if (Displacement.Z <= 0 && ElapsedTime > 0.1f)
	{
		float t = (-Velocity.Z - sqrt(Velocity.Z * Velocity.Z - 2 * Gravity.Z * (StartPosition.Z * TargetPosition.Z))) / Gravity.Z;
		Position = StartPosition + Velocity * t + 0.5f * Gravity * t * t;
		IsLaunched = false;
		// 도착
	}

}

void ArcKinematicController::LaunchAnimation()
{
	if(Owner == nullptr)
	{
		return;
	}
	// 나중에 실제 발사체 액터 만들어서 작업하자
	APlayerCharacter* Player = Cast<APlayerCharacter>(Owner);
	if (Player != nullptr)
	{
		//나중에 여기서 작업하자
	}
}

void ArcKinematicController::StopLaunchAnimation()
{
	if (Owner == nullptr)
	{
		return;
	}
	// 나중에 실제 발사체 액터 만들어서 작업하자
	APlayerCharacter* Player = Cast<APlayerCharacter>(Owner);
	if (Player != nullptr)
	{
		//나중에 여기서 작업하자
	}
}

bool ArcKinematicController::CalculateTrajectory()
{
	FVector ToTarget = TargetPosition - StartPosition;
	float HorizontalDistance = FVector(ToTarget.X, ToTarget.Y, 0.0f).Size();
	float VerticalDistance = ToTarget.Z;

	// 수평 거리가 0이면 발사 불가능
	if (HorizontalDistance == 0.0f)
	{
		return false;
	}
	// 이 부분 최적화 가능할거 같긴한데 위에서 CalculateInitialSpeed에서 이미 계산했다면 바로 발사도 가능할것 같긴한데 코드 더러워질듯 걍하자
	float Angle = CalculateLaunchAngle(HorizontalDistance, VerticalDistance, Speed, FMath::Abs(Gravity.Z));

	// 발사 각도가 NaN이면 발사 불가능
	if (isnan(Angle))
		return false;

	UE_LOG(LogTemp, Warning, TEXT("Angle : %f  Speed : %f"), (Angle * 180 / PI), Speed);
	UE_LOG(LogTemp, Warning, TEXT("Gravity %s"), *Gravity.ToString());
	Launch(Angle);

	return true;
}

float ArcKinematicController::CalculateLaunchAngle(float HorizontalDistance, float Height, float SpeedScalar, float GravityZ)
{
	float SpeedSquare = SpeedScalar * SpeedScalar;
	float Root = sqrt(SpeedSquare * SpeedSquare - GravityZ * (GravityZ * HorizontalDistance * HorizontalDistance + 2 * Height * SpeedSquare));
	float Term1 = SpeedSquare + Root;
	float Term2 = GravityZ * HorizontalDistance;
	return atan2(Term1, Term2);
}

float ArcKinematicController::CalculateInitialSpeed(FVector Start, FVector Target, float Angle, FVector FGravity)
{
	FVector ToTarget = Target - Start;
	float HorizontalDistance = FVector(ToTarget.X, ToTarget.Y, 0.0f).Size();
	float VerticalDistance = ToTarget.Z;
	float AngleInRadian = FMath::DegreesToRadians(Angle);
	float CosAngle = cos(AngleInRadian);
	float SinAngle = sin(AngleInRadian);
	float TanAngle = tan(AngleInRadian);
	float SpeedSquare = (HorizontalDistance * HorizontalDistance * FGravity.Z) / (2 * CosAngle * CosAngle * (VerticalDistance - TanAngle * HorizontalDistance));
	return sqrt(abs(SpeedSquare));
}

void ArcKinematicController::Launch(float Radian)
{
	FVector ToTarget = TargetPosition - StartPosition;
	FVector HorizontalDirection = FVector(ToTarget.X, ToTarget.Y, 0.0f).GetSafeNormal();
	Velocity = HorizontalDirection * Speed * cos(Radian);
	Velocity.Z = Speed * sin(Radian);
	IsLaunched = true;
}

