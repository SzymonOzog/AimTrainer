// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AimTrainerGameMode.h"
#include "AimTrainerHUD.h"
#include "AimTrainerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAimTrainerGameMode::AAimTrainerGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AAimTrainerHUD::StaticClass();
}
