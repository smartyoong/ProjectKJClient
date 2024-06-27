// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "CharacterPresetView.generated.h"

UCLASS()
class PROJECTKJCLIENT_API ACharacterPresetView : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterPresetView();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	USceneComponent* DefaultSceneComponent;

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, Category = "PresetModel")
	USkeletalMeshComponent* PresetModel;
	UPROPERTY(EditAnywhere, Category = "PresetModelView")
	USceneCaptureComponent2D* PresetModelView;
};
