// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPresetView.h"

// Sets default values
ACharacterPresetView::ACharacterPresetView()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	RootComponent = DefaultSceneComponent;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	PresetModel = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PresetModel"));
	PresetModel->SetupAttachment(SceneComponent);
	PresetModelView = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PresetModelView"));
}

// Called when the game starts or when spawned
void ACharacterPresetView::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterPresetView::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

