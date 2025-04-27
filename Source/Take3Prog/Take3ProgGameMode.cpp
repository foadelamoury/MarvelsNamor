// Copyright Epic Games, Inc. All Rights Reserved.

#include "Take3ProgGameMode.h"
#include "Take3ProgCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATake3ProgGameMode::ATake3ProgGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
