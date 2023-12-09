// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "STWPlayerState.h"
#include "STWWeaponBase.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "SantasToyWarfareCharacter.generated.h"

class ASTWWeaponBase;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USTWAttributesComponent;
struct FInputActionValue;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

class USTWActionComponent;

UCLASS(config=Game)
class ASantasToyWarfareCharacter : public ACharacter
{
	GENERATED_BODY()


	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
public:
	ASantasToyWarfareCharacter();

protected:
	virtual void BeginPlay();

public:

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

protected:

	UFUNCTION(BlueprintCallable)
	void SpawnWeapon();

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<ASTWWeaponBase> WeaponClass;

	UPROPERTY(Replicated)
	ASTWWeaponBase* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (ClampMin = 1, ClampMax = 2))
	float RunningSpeedMultiplier;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	USTWActionComponent* ActionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USTWAttributesComponent* AttributesComp;

	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	FGameplayTag SprintTag;

	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	FGameplayTag InteractTag;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	void StopMoving(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// Called when sprint input starts
	void SprintStart();

	// Called when sprint input is completed
	void SprintStop();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Utility")
	void OnCharacterPossesed();

	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	USTWActionComponent* GetActionComponent() const { return ActionComp; }

	ASTWWeaponBase* GetEquippedWeapon() const { return EquippedWeapon; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EPlayerTeam GetTeam() const;
};



