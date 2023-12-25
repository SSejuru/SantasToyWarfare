// Copyright Epic Games, Inc. All Rights Reserved.

#include "SantasToyWarfareCharacter.h"
#include "SantasToyWarfareProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "SantasToyWarfarePlayerController.h"
#include "STWActionComponent.h"
#include "STWAttributesComponent.h"
#include "STWGameState.h"
#include "STWWeaponBase.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ASantasToyWarfareCharacter

ASantasToyWarfareCharacter::ASantasToyWarfareCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	GiftMesh = CreateDefaultSubobject<UStaticMeshComponent>("Gift mesh");
	GiftMesh->SetupAttachment(GetMesh(), "GiftSocket");
	GiftMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GiftMesh->SetOwnerNoSee(true);

	ActionComp = CreateDefaultSubobject<USTWActionComponent>("ActionComp");
	AttributesComp = CreateDefaultSubobject<USTWAttributesComponent>("AttributesComp");

	bIsGiftMeshVisible = false;

	SetReplicates(true);
}

void ASantasToyWarfareCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	GiftMesh->SetVisibility(false);

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	ASTWGameState* GS = GetWorld()->GetGameState<ASTWGameState>();
	if(GS)
	{
		GS->OnGameEnded.AddDynamic(this, &ASantasToyWarfareCharacter::OnGameEnded);
	}
}

void ASantasToyWarfareCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(EquippedWeapon)
	{
		EquippedWeapon->SetLifeSpan(0.1f);
	}

	ASTWGameState* GS = GetWorld()->GetGameState<ASTWGameState>();
	if (GS)
	{
		GS->NotifyCharacterDestroyed(this);
	}
}

void ASantasToyWarfareCharacter::OnGameEnded(EPlayerTeam WinningTeam)
{
	ActionComp->bCanUseActions = false;
	SetCanBeDamaged(false);
	CustomTimeDilation = 0.5f;
}

//////////////////////////////////////////////////////////////////////////// Input



void ASantasToyWarfareCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASantasToyWarfareCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ASantasToyWarfareCharacter::StopMoving);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASantasToyWarfareCharacter::Look);

		//Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ASantasToyWarfareCharacter::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASantasToyWarfareCharacter::SprintStop);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

EPlayerTeam ASantasToyWarfareCharacter::GetTeam() const
{
	ASTWPlayerState* PS = GetPlayerState<ASTWPlayerState>();

	if (PS)
	{
		return PS->AssignedTeam;
	}

	return EPlayerTeam();
}

void ASantasToyWarfareCharacter::SetGiftVisibility(bool Visibility)
{
	bIsGiftMeshVisible = Visibility;
	OnRep_UpdateGiftVisibility();
}

void ASantasToyWarfareCharacter::OnRep_UpdateGiftVisibility()
{
	GiftMesh->SetVisibility(bIsGiftMeshVisible);
}




void ASantasToyWarfareCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ASantasToyWarfareCharacter::StopMoving(const FInputActionValue& Value)
{
	SprintStop();
}

void ASantasToyWarfareCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASantasToyWarfareCharacter::SprintStart()
{
	ActionComp->StartActionByTag(this, SprintTag);
}

void ASantasToyWarfareCharacter::SprintStop()
{
	ActionComp->StopActionByTag(this, SprintTag);
}

void ASantasToyWarfareCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ASantasToyWarfareCharacter::GetHasRifle()
{
	return bHasRifle;
}

FVector ASantasToyWarfareCharacter::GetPawnViewLocation() const
{
	return FirstPersonCameraComponent->GetComponentLocation();
}

void ASantasToyWarfareCharacter::SpawnWeapon()
{
	//Spawn Weapon
	if (WeaponClass)
	{
		FTransform SpawnTM;
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.Owner = this;

		ASTWWeaponBase* Weapon = GetWorld()->SpawnActor<ASTWWeaponBase>(WeaponClass, SpawnTM, SpawnParameters);
		EquippedWeapon = Weapon;
	}
}

void ASantasToyWarfareCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASantasToyWarfareCharacter, EquippedWeapon);
	DOREPLIFETIME(ASantasToyWarfareCharacter, bIsGiftMeshVisible);
}