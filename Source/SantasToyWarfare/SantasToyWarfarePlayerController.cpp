// Copyright Epic Games, Inc. All Rights Reserved.


#include "SantasToyWarfarePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "SantasToyWarfareCharacter.h"
#include "STWMainHUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include <EnhancedInputComponent.h>



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

void ASantasToyWarfarePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ASantasToyWarfarePlayerController::TogglePauseMenu);
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

void ASantasToyWarfarePlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());

		return;
	}

	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

EPlayerTeam ASantasToyWarfarePlayerController::GetPlayerTeam()
{
	ASTWPlayerState* PS = Cast<ASTWPlayerState>(PlayerState);

	if(PS)
	{
		return PS->AssignedTeam.GetValue();
	}

	return EPlayerTeam();
}

