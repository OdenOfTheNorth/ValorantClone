// Copyright Epic Games, Inc. All Rights Reserved.

#include "ValorantGameMode.h"
#include "ValorantHUD.h"
#include "ValorantCharacter.h"
#include "UObject/ConstructorHelpers.h"

AValorantGameMode::AValorantGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	//HUDClass = AValorantHUD::StaticClass();
}
