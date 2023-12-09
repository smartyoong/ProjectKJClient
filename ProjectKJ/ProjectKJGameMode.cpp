// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectKJGameMode.h"
#include "ProjectKJCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectKJGameMode::AProjectKJGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
