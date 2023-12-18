// Copyright Epic Games, Inc. All Rights Reserved.


#include "SantasToyWarfarePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "SantasToyWarfareCharacter.h"
#include "STWGameState.h"
#include "STWMainHUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ASantasToyWarfarePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);

		UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	}
}

void ASantasToyWarfarePlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	ASantasToyWarfareCharacter* PlayerCharacter = Cast<ASantasToyWarfareCharacter>(P);

	if (PlayerCharacter)
	{
		PlayerCharacter->OnCharacterPossesed();
	}

	if(!PlayerHUD)
	{
		PlayerHUD = CreateWidget<USTWMainHUDWidget>(this, HUDWidget);
		PlayerHUD->AddToViewport();
	}
	else
	{
		PlayerHUD->RestartWidget();
	}
}

EPlayerTeam ASantasToyWarfarePlayerController::GetPlayerTeam()
{
	ASTWPlayerState* GS = GetPlayerState<ASTWPlayerState>();

	if(GS)
	{
		return GS->AssignedTeam;
	}

	return EPlayerTeam();
}
