// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/STWAction.h"
#include "STWAction_Shoot.generated.h"

class ASTWProjectileBase;

/**
 * 
 */
UCLASS()
class SANTASTOYWARFARE_API USTWAction_Shoot : public USTWAction
{
	GENERATED_BODY()

protected:

	virtual void StartAction_Implementation(AActor* Instigator) override;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<ASTWProjectileBase> ProjectileClass;
};
