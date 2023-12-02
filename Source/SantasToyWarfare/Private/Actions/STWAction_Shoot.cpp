// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/STWAction_Shoot.h"
#include "Actions/STWAction_Shoot.h"

#include "STWWeaponBase.h"
#include "SantasToyWarfare/SantasToyWarfareCharacter.h"
#include "SantasToyWarfare/Public/STWProjectileBase.h"


void USTWAction_Shoot::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ASantasToyWarfareCharacter* Character = Cast<ASantasToyWarfareCharacter>(Instigator);

	if (!Character) return;

	// Try and fire a projectile
	if (ProjectileClass && Character->HasAuthority())
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position

			ASTWWeaponBase* Weapon = Character->GetEquippedWeapon();
			FVector FirePointLocation;

			if(Weapon)
			{
				FirePointLocation = Weapon->GetFireSocketLocation();
			}
			else
			{
				FVector MuzzleOffset;
				MuzzleOffset.X = 100.0f;

				FirePointLocation = Character->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
			}

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			ActorSpawnParams.Instigator = Character;

			// Spawn the projectile at the muzzle
			World->SpawnActor<ASTWProjectileBase>(ProjectileClass, FirePointLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	
}
