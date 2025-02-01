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
	PresetModelView->SetupAttachment(RootComponent);

	PresetModelBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PresetModelBody"));
	PresetModelFace = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PresetModelFace"));
	PresetModelTorso = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PresetModelTorso"));
	PresetModelLegs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PresetModelLegs"));
	PresetModelFeet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PresetModelFeet"));

	PresetModelBody->SetupAttachment(RootComponent);
	PresetModelFace->SetupAttachment(PresetModelBody);
	PresetModelFeet->SetupAttachment(PresetModelBody);
	PresetModelLegs->SetupAttachment(PresetModelBody);
	PresetModelTorso->SetupAttachment(PresetModelBody);

	PresetModelBeard = CreateDefaultSubobject<UGroomComponent>(TEXT("PresetModelBeard"));
	PresetModelEyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("PresetModelEyebrows"));
	PresetModelEyelashes = CreateDefaultSubobject<UGroomComponent>(TEXT("PresetModelEyelashes"));
	PresetModelFuzz = CreateDefaultSubobject<UGroomComponent>(TEXT("PresetModelFuzz"));
	PresetModelMustache = CreateDefaultSubobject<UGroomComponent>(TEXT("PresetModelMustache"));
	PresetModelHair = CreateDefaultSubobject<UGroomComponent>(TEXT("PresetModelHair"));

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

