// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownProjectGameMode.h"
#include "TopDownProjectPlayerController.h"
#include "TopDownProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATopDownProjectGameMode::ATopDownProjectGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATopDownProjectPlayerController::StaticClass();

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
