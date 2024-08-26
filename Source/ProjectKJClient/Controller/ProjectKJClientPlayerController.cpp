// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectKJClientPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "ProjectKJClientCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AProjectKJClientPlayerController::AProjectKJClientPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AProjectKJClientPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AProjectKJClientPlayerController::SetupInputComponent()
{
	//여기는 이제 UI같은 캐릭터랑 상관 없는 부분을 담당한다.
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// 매핑 문맥 추가 (현재는 일반 인간 모드 밖에 예정되어 있다 운전같은건 없을거니까,,)
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// UI 관련된건 여기로 옮기자
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AProjectKJClientPlayerController::OnInputStarted);
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AProjectKJClientPlayerController::OnSetDestinationTriggered);
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AProjectKJClientPlayerController::OnSetDestinationReleased);
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AProjectKJClientPlayerController::OnSetDestinationReleased);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AProjectKJClientPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AProjectKJClientPlayerController::OnSetDestinationTriggered()
{
	// 짧은 클릭인지 판별하기 위해 시간초 체크
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// 터치 모드 여부에 따라서 클릭한 곳 or 마우스 커서의 위치를 가져온다.
	FHitResult Hit;
	bool bHitSuccessful = false;

	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	// 정상적인 곳으로 커서 클릭을 했다, 캐싱해둔다.
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// 마우스 커서 방향으로 일단 이동
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AProjectKJClientPlayerController::OnSetDestinationReleased()
{
	// 짧게 누른게 맞으면 이동 (BP에서 설정)
	if (FollowTime <= ShortPressThreshold)
	{
		// 움직이는 건데, 아마 서버 작업이랑 진행한다하면, 여기를 다른걸로 바꿔야함
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		// 커서를 누른 위치에 소환
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}
