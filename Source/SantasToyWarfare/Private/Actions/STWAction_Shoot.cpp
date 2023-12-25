// Fill out your copyright notice in the Description page of Project Settings.


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
	if (ensureAlways(ProjectileClass) && Character->HasAuthority())
	{
		//Projectile Spawn location
		FVector FirePointLocation = Character->GetEquippedWeapon()->GetFireSocketLocation();

		//Line trace from camera to world to find hit location
		FHitResult Hit;
		FVector Start = Character->GetPawnViewLocation();
		FVector Forward = Character->GetControlRotation().Vector();
		FVector End = Start + (Forward * 2500.0f);

		FCollisionShape Shape;
		Shape.SetSphere(15.0f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Character);

		FCollisionObjectQueryParams CollisionObjectParams;

		CollisionObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		CollisionObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
		CollisionObjectParams.AddObjectTypesToQuery(ECC_Pawn);

		bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, Start, End, FQuat::Identity, CollisionObjectParams, Shape, Params);

		//FColor lineColor = bBlockingHit ? FColor::Green : FColor::Red;
		//DrawDebugLine(GetWorld(), Start, End, lineColor, false, 2, 0, 2.0f);

		FVector HitPosition = bBlockingHit ? Hit.ImpactPoint : End;

		//Spawn Projectile in hand location with rotation to line trace hit position
		FVector TargetDirection = HitPosition - FirePointLocation;
		FRotator ProjectileRotation = FRotationMatrix::MakeFromX(TargetDirection).Rotator();

		FTransform SpawnTM = FTransform(ProjectileRotation, FirePointLocation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = Character;

		GetWorld()->SpawnActor<ASTWProjectileBase>(ProjectileClass, SpawnTM, SpawnParams);
	}

	
}
