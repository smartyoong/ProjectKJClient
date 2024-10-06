// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KinematicController.h"

/**
 * 
 */
class PROJECTKJCLIENT_API SeperateMethod : public Behaviors
{
private:
	TArray<KinematicStatic>* Targets;
	const float Threshold = 50.f;
	const float DecayCoefficient = 3.f;
public:
	SeperateMethod(TArray<KinematicStatic>* TargetList);
	virtual ~SeperateMethod() = default;
	virtual std::optional<SteeringHandle> GetSteeringHandle(float Ratio, KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate,
		float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget) override;
};
