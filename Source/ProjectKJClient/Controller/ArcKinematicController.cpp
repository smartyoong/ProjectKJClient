// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/ArcKinematicController.h"
#include "GameFramework/Actor.h"


ArcKinematicController::ArcKinematicController(AActor* Owner, FVector Start, FVector End, float Speed, FVector Gravity)
{
	this->Owner = Owner;
	this->Position = Start;
	this->TargetPosition = End;
	this->Speed = Speed;
	this->Arrived = false;
	this->Radius = 1.f;
	this->Velocity = FVector::ZeroVector;
	this->Gravity = Gravity;
}

void ArcKinematicController::Update(float DeltaTime)
{
	if (Arrived)
	{
		return;
	}

	if(Owner)
		Owner->SetActorLocation(Position);

	auto Value =  CalculateVelocity(Position, TargetPosition, Speed);

	if (Value.has_value())
	{
		Velocity = Value.value();
	}
	else
	{
		Arrived = true;
		UE_LOG(LogTemp, Warning, TEXT("Projectiile Cant Move When Move %s to %s"), *Position.ToString(), *TargetPosition.ToString());
		return;
	}

	Position += Velocity * DeltaTime;

	UE_LOG(LogTemp, Warning, TEXT("Projectile Move %s"), *Position.ToString());

	if (FVector::Dist(Position, TargetPosition) < Radius)
	{
		Arrived = true;
		UE_LOG(LogTemp, Warning, TEXT("Projectile Arrived %s"), *Position.ToString());
	}
}

bool ArcKinematicController::CheckCanShoot()
{
	auto Value = CalculateVelocity(Position, TargetPosition, Speed);
	return Value.has_value();
}

std::optional<FVector> ArcKinematicController::CalculateVelocity(FVector Start, FVector End, float MuzzleV)
{
	FVector Delta = End - Start;
	float a = Gravity.SizeSquared();
	float b = -4 * (FVector::DotProduct(Delta, Gravity) + MuzzleV * MuzzleV);
	float c = 4 * Delta.SizeSquared();

	float D = b * b - 4 * a * c;
	if (D < 0)
	{
		return std::nullopt;
	}

	float t1 = FMath::Sqrt((-b + FMath::Sqrt(D)) / (2 * a));
	float t2 = FMath::Sqrt((-b - FMath::Sqrt(D)) / (2 * a));

	float ttt = 0.f;
	if (t1 < 0)
	{
		if (t2 < 0)
		{
			return std::nullopt;
		}
		else
		{
			ttt = t2;
		}
	}
	else
	{
		if (t2 < 0)
		{
			ttt = t1;
		}
		else
		{
			ttt = FMath::Min(t1, t2);
		}
	}

	return (Delta * 2 - Gravity * (ttt * ttt)) / (2 * MuzzleV * ttt);
}
