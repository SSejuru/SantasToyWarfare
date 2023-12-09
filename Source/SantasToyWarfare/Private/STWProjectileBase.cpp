// Fill out your copyright notice in the Description page of Project Settings.


#include "STWProjectileBase.h"

#include "STWAttributesComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SantasToyWarfare/SantasToyWarfareCharacter.h"

ASTWProjectileBase::ASTWProjectileBase()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	MeshComp->SetupAttachment(CollisionComp);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	bReplicates = true;
}

void ASTWProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	ASantasToyWarfareCharacter* Character = Cast<ASantasToyWarfareCharacter>(GetInstigator());

	if (Character)
	{
		InstigatorCharacter = Character;
	}
}

void ASTWProjectileBase::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		ASantasToyWarfareCharacter* Character = Cast<ASantasToyWarfareCharacter>(OtherActor);
		if (Character->GetTeam() != InstigatorCharacter->GetTeam())
		{
			USTWAttributesComponent* AttributeComp = USTWAttributesComponent::GetAttributes(OtherActor);
			if (AttributeComp)
			{
				AttributeComp->ApplyHealthChange(GetInstigator(), -InstigatorCharacter->GetEquippedWeapon()->GetBulletDamage());
			}

			Destroy();
		}
	}
}

void ASTWProjectileBase::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}



void ASTWProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASTWProjectileBase::OnComponentOverlap); // set up a notification for when this component hits something 
	CollisionComp->OnComponentHit.AddDynamic(this, &ASTWProjectileBase::OnComponentHit);
}

