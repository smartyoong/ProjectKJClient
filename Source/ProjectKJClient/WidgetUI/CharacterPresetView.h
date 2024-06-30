// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GroomComponent.h"
#include "GameFramework/Actor.h"
#include "CharacterPresetView.generated.h"

UCLASS()
class PROJECTKJCLIENT_API ACharacterPresetView : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterPresetView();

private:


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	USceneComponent* DefaultSceneComponent;

	UPROPERTY(EditAnywhere, Category = "PresetModelView")
	USceneCaptureComponent2D* PresetModelView;
	UPROPERTY(EditAnywhere, Category = "PresetModel")
	USkeletalMeshComponent* PresetModelBody;
	UPROPERTY(EditAnywhere, Category = "PresetModel")
	USkeletalMeshComponent* PresetModelFeet;
	UPROPERTY(EditAnywhere, Category = "PresetModel")
	USkeletalMeshComponent* PresetModelLegs;
	UPROPERTY(EditAnywhere, Category = "PresetModel")
	USkeletalMeshComponent* PresetModelTorso;
	UPROPERTY(EditAnywhere, Category = "PresetModel")
	USkeletalMeshComponent* PresetModelFace;

	UPROPERTY(EditAnywhere, Category = "PresetModel")
	UGroomComponent* PresetModelHair;
	UPROPERTY(EditAnywhere, Category = "PresetModel")
	UGroomComponent* PresetModelBeard;
	UPROPERTY(EditAnywhere, Category = "PresetModel")
	UGroomComponent* PresetModelEyebrows;
	UPROPERTY(EditAnywhere, Category = "PresetModel")
	UGroomComponent* PresetModelEyelashes;
	UPROPERTY(EditAnywhere, Category = "PresetModel")
	UGroomComponent* PresetModelFuzz;
	UPROPERTY(EditAnywhere, Category = "PresetModel")
	UGroomComponent* PresetModelMustache;
};
