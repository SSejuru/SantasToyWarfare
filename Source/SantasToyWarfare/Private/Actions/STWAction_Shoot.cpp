// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/STWAction_Shoot.h"
#include "Actions/STWAction_Shoot.h"

#include "STWWeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "SantasToyWarfare/SantasToyWarfareCharacter.h"
#include "SantasToyWarfare/TP_WeaponComponent.h"
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
				FirePointLocation = Weapon->GetWeaponComponent()->GetFireSocketLocation();
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

			// Spawn the projectile at the muzzle
			World->SpawnActor<ASTWProjectileBase>(ProjectileClass, FirePointLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// Try and play a firing animation if specified
	if (FirstPersonFireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FirstPersonFireAnimation, 1.f);
		}
	}

	if (ensure(ThirdPersonFireAnimation))
	{
		Character->PlayAnimMontage(ThirdPersonFireAnimation);
	}
}
