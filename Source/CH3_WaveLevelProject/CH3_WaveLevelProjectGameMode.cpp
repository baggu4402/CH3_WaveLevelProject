// Copyright Epic Games, Inc. All Rights Reserved.

#include "CH3_WaveLevelProjectGameMode.h"
#include "CH3_WaveLevelProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACH3_WaveLevelProjectGameMode::ACH3_WaveLevelProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
