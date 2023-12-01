// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STWWeaponBase.generated.h"

class ASantasToyWarfareCharacter;
class UTP_WeaponComponent;

UCLASS()
class SANTASTOYWARFARE_API ASTWWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTWWeaponBase();

protected:

	UPROPERTY(VisibleAnywhere, Category= Components)
	UTP_WeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, Category = Components)
	USkeletalMeshComponent* MeshComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AttachWeapon(ASantasToyWarfareCharacter* TargetCharacter);

	UTP_WeaponComponent* GetWeaponComponent() const { return WeaponComponent; }
};
