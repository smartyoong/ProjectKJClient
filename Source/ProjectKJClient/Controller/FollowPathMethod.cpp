// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/FollowPathMethod.h"
#include "Controller/EqualVelocityChaseMethod.h"
#include "PlayerCharacter.h"

FollowPathMethod::FollowPathMethod(AActor* Sender ,PathComponent* PathComponent) : Sender(Sender), Path(PathComponent)
{
}

std::optional<SteeringHandle> FollowPathMethod::GetSteeringHandle(float Ratio, KinematicStatic Character, KinematicStatic Target, float MaxSpeed, float MaxAccelerate, float MaxRotation, float MaxAngular, float TargetRadius, float SlowRadius, float TimeToTarget)
{
    KinematicStatic TargetPoint = Target;
    int CurrentIndex = Path->GetCurrentIndex();
    TargetPoint.Position = Path->GetPosition(CurrentIndex);
    if (Path->Arrived(Character.Position))
    {
        CurrentIndex = Path->GetNextPositionIndex();
        // 여기가 마지막이다.
		if (CurrentIndex == -1)
		{
			return std::nullopt;
		}
        TargetPoint.Position = Path->GetPosition(CurrentIndex);

        // 사람이라면 이동 패킷을 보낸다.
        if (Cast<APlayerCharacter>(Sender))
        {
			Cast<APlayerCharacter>(Sender)->SendMovePacket(TargetPoint.Position);
        }
    }

    EqualVelocityChaseMethod Chase;
    return Chase.GetSteeringHandle(Ratio, Character, TargetPoint, MaxSpeed, MaxAccelerate, MaxRotation, MaxAngular, TargetRadius, SlowRadius, TimeToTarget);
}
