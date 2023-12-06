// Fill out your copyright notice in the Description page of Project Settings.


#include "STWWeaponBase.h"

#include "EnhancedInputSubsystems.h"
#include "STWActionComponent.h"
#include "SantasToyWarfare/SantasToyWarfareCharacter.h"
#include "SantasToyWarfare/TP_WeaponComponent.h"
#include <EnhancedInputComponent.h>

#include "Kismet/GameplayStatics.h"

// Sets default values
ASTWWeaponBase::ASTWWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>("First Person Mesh Comp");
	RootComponent = FirstPersonMesh;
	FirstPersonMesh->SetOnlyOwnerSee(true);

	ThirdPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Third Person Mesh Comp");
	ThirdPersonMesh->SetupAttachment(FirstPersonMesh);
	ThirdPersonMesh->SetOwnerNoSee(true);

	FireRate = 0.1f;
	BulletDamage = 20.0f;
	FireSocketName = "FirePoint";

	SetReplicates(true);
}



// Called when the game starts or when spawned
void ASTWWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ASantasToyWarfareCharacter>(GetOwner());;
	AttachWeaponToOwner();
}

// Called every frame
void ASTWWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTWWeaponBase::AttachWeaponToOwner()
{
	if (OwnerCharacter)
	{
		// Attach the weapon mesh to the first and third person characters
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

		FirstPersonMesh->AttachToComponent(OwnerCharacter->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
		ThirdPersonMesh->AttachToComponent(OwnerCharacter->GetMesh(), AttachmentRules, FName(TEXT("GripPoint")));

		OwnerCharacter->SetHasRifle(true);

		//Add shooting Action to Character and bindings
		OwnerCharacter->GetActionComponent()->AddAction(OwnerCharacter, ShootingAction);

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
				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ASTWWeaponBase::Fire);
			}
		}
	}
}



void ASTWWeaponBase::Fire()
{
	if(OwnerCharacter->GetActionComponent()->StartActionByTag(OwnerCharacter, ShootTag))
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Fire, this, &ASTWWeaponBase::StopFireAction, FireRate);

		if (OwnerCharacter->HasAuthority())
		{
			MulticastFireFeedback();
		}
		else
		{
			ServerFireFeedback();
			PlayFireFeedback();
		}
	}
}

void ASTWWeaponBase::StopFireAction()
{
	OwnerCharacter->GetActionComponent()->StopActionByTag(OwnerCharacter, ShootTag);
}

void ASTWWeaponBase::PlayFireFeedback()
{
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, OwnerCharacter->GetActorLocation());
	}

	// Try and play a firing animation if specified
	if (ensure(FirstPersonFireAnimation))
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FirstPersonFireAnimation, 1.f);
		}
	}

	if (ensure(ThirdPersonFireAnimation))
	{
		OwnerCharacter->PlayAnimMontage(ThirdPersonFireAnimation);
	}
}

void ASTWWeaponBase::ServerFireFeedback_Implementation()
{
	PlayFireFeedback();
}


void ASTWWeaponBase::MulticastFireFeedback_Implementation()
{
	PlayFireFeedback();
}

void ASTWWeaponBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

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

