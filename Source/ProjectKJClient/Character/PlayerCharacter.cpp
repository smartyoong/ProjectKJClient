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
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ProjectKJClientPlayerController.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "MainGameInstance.h"

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

void APlayerCharacter::UpdateMove(float DeltaTime)
{
	if (IsMoving)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector Direction = (MoveDestination - CurrentLocation).GetSafeNormal();
		FVector NewLocation = CurrentLocation + (Direction * Speed * DeltaTime);

		// 목표 위치에 도달했는지 확인
		if (FVector::Dist(NewLocation, MoveDestination) <= Speed * DeltaTime)
		{
			NewLocation = MoveDestination;
			IsMoving = false;
		}
		SetActorRotation(Direction.Rotation());
		SetActorLocation(NewLocation);
		UE_LOG(LogTemp, Warning, TEXT("CurrentLocation: %s"), *NewLocation.ToString());
	}
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
	UpdateMove(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 나중에 컨트롤러에 있는 메서드를 여기로 옮겨야할 수 있음
		// 캐릭터 관련된건 여기로 옮기자
		
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &APlayerCharacter::ClickAndMove);

		// 기본 입력 매핑 컨텍스트를 추가합니다.
		if (AProjectKJClientPlayerController* PlayerController = Cast<AProjectKJClientPlayerController>(GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 1);
			}
		}
	}
}

void APlayerCharacter::SetSpawnBaseInfo(FCharacterInfo Info)
{
	AccountID = Info.AccountID;
	NickName = Info.NickName;
	AuthHashCode = Info.AuthHashCode;
	CharacterPresetID = Info.CharacterPresetID;
	OldLocation = Info.FirstSpawnLocation;
	CurrentMapID = Info.SpawnMapID;
	Gender = Info.Gender;
	Job = Info.Job;
	JobLevel = Info.JobLevel;
	Level = Info.Level;
	EXP = Info.EXP;
}

void APlayerCharacter::MoveToLocation(FVector Location)
{
	OldLocation = GetActorLocation();
	MoveDestination = Location;
	IsMoving = true;
}

void APlayerCharacter::RollBackLocation()
{
	if (OldLocation != FVector::ZeroVector)
		SetActorLocation(OldLocation);
	IsMoving = false;
}

void APlayerCharacter::ClickAndMove()
{
	FHitResult Hit;
	bool HitSuccessful = false;
	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::RightMouseButton))
	{
		HitSuccessful = GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	}
	if (HitSuccessful)
	{
		Cast<UMainGameInstance>(GetGameInstance())->SendPacketToGameServer(GamePacketListID::REQUEST_MOVE,FRequestMovePacket(AccountID,AuthHashCode,CurrentMapID,Hit.Location.X,Hit.Location.Y));
		// Z축 보정
		Hit.Location.Z = GetActorLocation().Z;
		MoveToLocation(Hit.Location);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, Hit.Location, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
}

