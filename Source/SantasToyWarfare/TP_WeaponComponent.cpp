// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "SantasToyWarfareCharacter.h"
#include "SantasToyWarfareProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "STWActionComponent.h"
#include "Actions/STWAction.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

	FireSocketName = "FirePoint";

	SetIsReplicatedByDefault(true);
}



void UTP_WeaponComponent::Fire()
{
	if (OwnerCharacter == nullptr || OwnerCharacter->GetController() == nullptr)
	{
		return;
	}

	OwnerCharacter->GetActionComponent()->StartActionByTag(OwnerCharacter, FGameplayTag::RequestGameplayTag("Action.Shoot"));

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Fire, this, &UTP_WeaponComponent::StopFireAction, FireRate);
}

void UTP_WeaponComponent::StopFireAction()
{
	OwnerCharacter->GetActionComponent()->StopActionByTag(OwnerCharacter, FGameplayTag::RequestGameplayTag("Action.Shoot"));
}


void UTP_WeaponComponent::AttachWeapon(ASantasToyWarfareCharacter* TargetCharacter)
{
	OwnerCharacter = TargetCharacter;

	// Check that the character is valid, and has no rifle yet
	if (OwnerCharacter == nullptr || OwnerCharacter->GetHasRifle())
	{
		return;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(OwnerCharacter->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	
	// switch bHasRifle so the animation blueprint can switch to another animation set
	OwnerCharacter->SetHasRifle(true);

	//Add Shooting Action
	OwnerCharacter->GetActionComponent()->AddAction(OwnerCharacter, ShootingAction);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
		}
	}
}



void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}


