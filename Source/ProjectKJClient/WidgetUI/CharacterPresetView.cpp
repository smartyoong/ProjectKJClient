// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPresetView.h"

// Sets default values
ACharacterPresetView::ACharacterPresetView()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	RootComponent = DefaultSceneComponent;
	PresetModelView = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PresetModelView"));

	PresetModelBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PresetModelBody"));
	PresetModelFeet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PresetModelFeet"));
	PresetModelLegs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PresetModelLegs"));
	PresetModelTorso = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PresetModelTorso"));
	PresetModelFace = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PresetModelFace"));

	PresetModelBody->SetupAttachment(RootComponent);
	PresetModelFace->SetupAttachment(PresetModelBody);
	PresetModelFeet->SetupAttachment(PresetModelBody);
	PresetModelLegs->SetupAttachment(PresetModelBody);
	PresetModelTorso->SetupAttachment(PresetModelBody);

	PresetModelHair = CreateDefaultSubobject<UGroomComponent>(TEXT("PresetModelHair"));
	PresetModelBeard = CreateDefaultSubobject<UGroomComponent>(TEXT("PresetModelBeard"));
	PresetModelEyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("PresetModelEyebrows"));
	PresetModelEyelashes = CreateDefaultSubobject<UGroomComponent>(TEXT("PresetModelEyelashes"));
	PresetModelFuzz = CreateDefaultSubobject<UGroomComponent>(TEXT("PresetModelFuzz"));
	PresetModelMustache = CreateDefaultSubobject<UGroomComponent>(TEXT("PresetModelMustache"));

	PresetModelBeard->SetupAttachment(PresetModelFace);
	PresetModelEyebrows->SetupAttachment(PresetModelFace);
	PresetModelEyelashes->SetupAttachment(PresetModelFace);
	PresetModelFuzz->SetupAttachment(PresetModelFace);
	PresetModelMustache->SetupAttachment(PresetModelFace);
	PresetModelHair->SetupAttachment(PresetModelFace);
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

