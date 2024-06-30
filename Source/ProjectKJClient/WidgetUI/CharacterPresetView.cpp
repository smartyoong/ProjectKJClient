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
	PresetModelFace->AttachToComponent(PresetModelBody, FAttachmentTransformRules::SnapToTargetIncludingScale);
	PresetModelFeet->AttachToComponent(PresetModelBody, FAttachmentTransformRules::SnapToTargetIncludingScale);
	PresetModelLegs->AttachToComponent(PresetModelBody, FAttachmentTransformRules::SnapToTargetIncludingScale);
	PresetModelTorso->AttachToComponent(PresetModelBody, FAttachmentTransformRules::SnapToTargetIncludingScale);

	PresetModelHair = CreateDefaultSubobject<UGroomComponent>(TEXT("PresetModelHair"));
	PresetModelBeard = CreateDefaultSubobject<UGroomComponent>(TEXT("PresetModelBeard"));
	PresetModelEyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("PresetModelEyebrows"));
	PresetModelEyelashes = CreateDefaultSubobject<UGroomComponent>(TEXT("PresetModelEyelashes"));
	PresetModelFuzz = CreateDefaultSubobject<UGroomComponent>(TEXT("PresetModelFuzz"));
	PresetModelMustache = CreateDefaultSubobject<UGroomComponent>(TEXT("PresetModelMustache"));

	PresetModelBeard->AttachToComponent(PresetModelFace, FAttachmentTransformRules::SnapToTargetIncludingScale);
	PresetModelEyebrows->AttachToComponent(PresetModelFace, FAttachmentTransformRules::SnapToTargetIncludingScale);
	PresetModelEyelashes->AttachToComponent(PresetModelFace, FAttachmentTransformRules::SnapToTargetIncludingScale);
	PresetModelFuzz->AttachToComponent(PresetModelFace, FAttachmentTransformRules::SnapToTargetIncludingScale);
	PresetModelMustache->AttachToComponent(PresetModelFace, FAttachmentTransformRules::SnapToTargetIncludingScale);
	PresetModelHair->AttachToComponent(PresetModelFace, FAttachmentTransformRules::SnapToTargetIncludingScale);
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

