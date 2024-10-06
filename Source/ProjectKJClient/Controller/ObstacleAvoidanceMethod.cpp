// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/ObstacleAvoidanceMethod.h"
#include "ChaseMethod.h"

ObstacleAvoidanceMethod::ObstacleAvoidanceMethod(AActor* Owner) : MySelf(Owner)
{
}

std::optional<SteeringHandle> ObstacleAvoidanceMethod::GetSteeringHandle(float Ratio, KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate, float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget)
{
	if (MySelf == nullptr)
		return std::nullopt;
	FVector Ray = Character.Velocity;
	Ray.Normalize();
	Ray *= LookAhead;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(MySelf);

	bool IsHit = MySelf->GetWorld()->LineTraceSingleByChannel(HitResult, Character.Position, (Character.Position + Ray), ECC_Visibility, CollisionParameters);

	if (IsHit)
	{
		FVector HitPoint = HitResult.ImpactPoint;
		HitPoint += HitResult.ImpactNormal * AvoidanceRadius;
		KinematicStatic NewTarget;
		NewTarget = Target;
		NewTarget.Position = HitPoint;
		return ChaseMethod().GetSteeringHandle(Ratio, Character, NewTarget, MaxSpeed, MaxAccelerate, MaxRotation, MaxAngular, TargetRadius, SlowRadius, TimeToTarget);
	}
	else
		return std::nullopt;
}
