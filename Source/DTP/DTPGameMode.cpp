// Copyright Epic Games, Inc. All Rights Reserved.

#include "DTPGameMode.h"
#include "DTPPlayerController.h"
#include "DTPCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADTPGameMode::ADTPGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ADTPPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}