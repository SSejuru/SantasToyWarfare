// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STWWeaponBase.generated.h"

class USTWAction;
class ASantasToyWarfareCharacter;
class UTP_WeaponComponent;

UCLASS()
class SANTASTOYWARFARE_API ASTWWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTWWeaponBase();

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions")
	TSubclassOf<USTWAction> ShootingAction;

protected:

	UPROPERTY()
	ASantasToyWarfareCharacter* OwnerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	FName FireSocketName;

	UPROPERTY(VisibleAnywhere, Category= Components)
	USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(VisibleAnywhere, Category = Components)
	USkeletalMeshComponent* ThirdPersonMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* FirstPersonFireAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* ThirdPersonFireAnimation;

	// Rate in which the weapon can shoot in seconds
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireRate;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BulletDamage;

	FTimerHandle TimerHandle_Fire;

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	UFUNCTION()
	void StopFireAction();

	UFUNCTION()
	void PlayFireFeedback();

	UFUNCTION(Server, Unreliable)
	void ServerFireFeedback();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastFireFeedback();

	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AttachWeaponToOwner();

	float GetBulletDamage() const { return BulletDamage; }

	FVector GetFireSocketLocation() const { return FirstPersonMesh->GetSocketLocation(FireSocketName); }
};
