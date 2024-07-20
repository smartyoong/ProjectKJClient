// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//SkeletalBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	SkeletalFace = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));
	SkeletalTorso = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Torso"));
	SkeletalLegs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	SkeletalFeet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Feet"));

	//SkeletalBody->SetupAttachment(GetMesh());
	SkeletalFace->SetupAttachment(GetMesh());
	SkeletalFeet->SetupAttachment(GetMesh());
	SkeletalLegs->SetupAttachment(GetMesh());
	SkeletalTorso->SetupAttachment(GetMesh());

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

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;
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

