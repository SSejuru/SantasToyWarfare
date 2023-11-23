// Copyright Epic Games, Inc. All Rights Reserved.

#include "SantasToyWarfareGameMode.h"
#include "SantasToyWarfareCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASantasToyWarfareGameMode::ASantasToyWarfareGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
