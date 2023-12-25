// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "STWPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "SantasToyWarfarePlayerController.generated.h"

class USTWMainHUDWidget;
class UInputMappingContext;

/**
 *
 */
UCLASS()
class SANTASTOYWARFARE_API ASantasToyWarfarePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category= Input)
	UInputAction* PauseAction;

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

	// Begin Actor interface
protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidget;

	UPROPERTY()
	USTWMainHUDWidget* PlayerHUD;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> PauseMenuInstance;

	virtual void BeginPlay() override;

	virtual void AcknowledgePossession(APawn* P) override;

	virtual void SetupInputComponent() override;

	// End Actor interface

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EPlayerTeam GetPlayerTeam();
};
