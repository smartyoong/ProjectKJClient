// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectKJClientGameMode.h"
#include "ProjectKJClientPlayerController.h"
#include "ProjectKJClientCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectKJClientGameMode::AProjectKJClientGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AProjectKJClientPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}