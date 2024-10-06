// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CollisionAvoidanceMethod.h"

CollisionAvoidanceMethod::CollisionAvoidanceMethod(TArray<KinematicStatic>* TargetList) : Targets(TargetList)
{
}

std::optional<SteeringHandle> CollisionAvoidanceMethod::GetSteeringHandle(float Ratio, KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate, float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget)
{
	if(Targets == nullptr)
	{
		return std::nullopt;
	}
	float ShortestTime = std::numeric_limits<float>::infinity();
	KinematicStatic FirstTarget;
	float FirstMinSeparation = 0.f;
	float FirstDistance = 0.f;
	FVector FirstRelativePos;
	FVector FirstRelativeVel;
	for (auto& t : *Targets)
	{
		FVector RelativePos = t.Position - Character.Position;
		FVector RelativeVel = t.Velocity - Character.Velocity;
		float RelativeSpeed = RelativeVel.Size();
		float TimeToCollision = FVector::DotProduct(RelativePos, RelativeVel) / (RelativeSpeed * RelativeSpeed);
		float Distance = RelativePos.Size();
		float MinSeparation = Distance - RelativeSpeed * TimeToCollision;
		if (MinSeparation > 2 * TargetRadius)
		{
			continue;
		}
		if (TimeToCollision > 0 && TimeToCollision < ShortestTime)
		{
			ShortestTime = TimeToCollision;
			FirstTarget = t;
			FirstMinSeparation = MinSeparation;
			FirstDistance = Distance;
			FirstRelativePos = RelativePos;
			FirstRelativeVel = RelativeVel;
		}
	}
	if (FirstTarget.Position.Size() == 0)
	{
		return std::nullopt;
	}
	FVector Pos;
	if (FirstMinSeparation <= 0 || FirstDistance < 2 * TargetRadius)
	{
		Pos = FirstTarget.Position - Character.Position;
	}
	else
	{
		Pos = FirstRelativePos + FirstRelativeVel * ShortestTime;
	}
	Pos.Normalize();
	SteeringHandle Result;
	Result.Linear = Pos * MaxAccelerate;
	Result.Angular = 0.f;
	return Result;
}
