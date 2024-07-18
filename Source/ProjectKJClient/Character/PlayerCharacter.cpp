// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	SkeletalFace = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));
	SkeletalTorso = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Torso"));
	SkeletalLegs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	SkeletalFeet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Feet"));

	SkeletalBody->SetupAttachment(RootComponent);
	SkeletalFace->SetupAttachment(SkeletalBody);
	SkeletalFeet->SetupAttachment(SkeletalBody);
	SkeletalLegs->SetupAttachment(SkeletalBody);
	SkeletalTorso->SetupAttachment(SkeletalBody);

	SkeletalBeard = CreateDefaultSubobject<UGroomComponent>(TEXT("Beard"));
	SkeletalEyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	SkeletalEyelashes = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyelashes"));
	SkeletalFuzz = CreateDefaultSubobject<UGroomComponent>(TEXT("Fuzz"));
	SkeletalMustache = CreateDefaultSubobject<UGroomComponent>(TEXT("Mustache"));
	SkeletalHair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));

	SkeletalBeard->SetupAttachment(SkeletalFace);
	SkeletalEyebrows->SetupAttachment(SkeletalFace);
	SkeletalEyelashes->SetupAttachment(SkeletalFace);
	SkeletalFuzz->SetupAttachment(SkeletalFace);
	SkeletalMustache->SetupAttachment(SkeletalFace);
	SkeletalHair->SetupAttachment(SkeletalFace);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

