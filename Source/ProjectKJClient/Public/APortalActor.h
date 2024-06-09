// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APortalActor.generated.h"

UCLASS()
class PROJECTKJCLIENT_API AAPortalActor : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere)
	int32 TargetMapID = -1;
	
public:	
	// Sets default values for this actor's properties
	AAPortalActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void SetTargetMapID(int32 MapID) { TargetMapID = MapID; }
	int32 GetTargetMapID() const { return TargetMapID; }
	void GetPortalBoxExtent(FVector& OutExtent) const;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
