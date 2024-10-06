// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KinematicController.h"
/**
 * 
 */
class PROJECTKJCLIENT_API LockOnMethod : public Behaviors
{
public:
	LockOnMethod()= default;
	virtual ~LockOnMethod() = default;
	virtual std::optional<SteeringHandle> GetSteeringHandle(float Ratio, KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate, float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget) override;
};