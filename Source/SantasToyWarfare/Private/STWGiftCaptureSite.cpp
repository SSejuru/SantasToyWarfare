// Fill out your copyright notice in the Description page of Project Settings.


#include "STWGiftCaptureSite.h"

#include "STWGameMode.h"
#include "STWGameState.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "SantasToyWarfare/SantasToyWarfareCharacter.h"

// Sets default values
ASTWGiftCaptureSite::ASTWGiftCaptureSite()
{
	TreeStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Tree Static Mesh");
	TreeStaticMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	RootComponent = TreeStaticMesh;

	GiftStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Gift Static Mesh");
	GiftStaticMesh->SetupAttachment(TreeStaticMesh);
	GiftStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BoxComp = CreateDefaultSubobject<UBoxComponent>("Box Collision Component");
	BoxComp->SetupAttachment(TreeStaticMesh);

	SphereComp = CreateDefaultSubobject<USphereComponent>("Sphere Collision Component");
	SphereComp->SetupAttachment(TreeStaticMesh);

	bHasGift = true;
	SetReplicates(true);
}

void ASTWGiftCaptureSite::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ASTWGiftCaptureSite::OnSiteBeginOverlap);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASTWGiftCaptureSite::OnGiftBeginOverlap);
}

// Called when the game starts or when spawned
void ASTWGiftCaptureSite::BeginPlay()
{
	Super::BeginPlay();
}

void ASTWGiftCaptureSite::OnSiteBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ASantasToyWarfareCharacter* Character = Cast<ASantasToyWarfareCharacter>(OtherActor);
		if (Character)
		{
			EPlayerTeam CharacterTeam = Character->GetTeam();

			if (CharacterTeam != TeamOwner) { return; }

			ASTWPlayerState* PlayerState = Character->GetPlayerState<ASTWPlayerState>();
			if (PlayerState && PlayerState->bIsCarryingGift)
			{
				Server_CaptureGift(Character);
				ASantasToyWarfarePlayerController* PC = Cast<ASantasToyWarfarePlayerController>(Character->GetController());
				if(PC && PC->IsLocalPlayerController())
				{
					//PC->CaptureGift();
				}
			}
		}
	}
}

void ASTWGiftCaptureSite::Server_CaptureGift_Implementation(ASantasToyWarfareCharacter* CapturingCharacter)
{
	ASTWGameMode* GM = GetWorld()->GetAuthGameMode<ASTWGameMode>();

	ASTWPlayerState* PS = CapturingCharacter->GetController()->GetPlayerState<ASTWPlayerState>();

	if (PS)
	{
		PS->bIsCarryingGift = false;
	}

	if (GM)
	{
		GM->CaptureGift(TeamOwner.GetValue());
		CapturingCharacter->SetGiftVisibility(false);
	}
}


void ASTWGiftCaptureSite::OnGiftBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && bHasGift)
	{
		ASantasToyWarfareCharacter* Character = Cast<ASantasToyWarfareCharacter>(OtherActor);

		if (Character)
		{
			EPlayerTeam CharacterTeam = Character->GetTeam();

			if (CharacterTeam == TeamOwner) { return; }

			Server_PickUpGift(Character);
		}
	}
}



void ASTWGiftCaptureSite::UpdateGiftVisibility()
{
	GiftStaticMesh->SetVisibility(bHasGift);
	SphereComp->SetCollisionEnabled(bHasGift ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}

void ASTWGiftCaptureSite::RestockGift()
{
	bHasGift = true;
	UpdateGiftVisibility();
}

void ASTWGiftCaptureSite::Server_PickUpGift_Implementation(ASantasToyWarfareCharacter* PickingCharacter)
{
	ASTWPlayerState* PlayerState = PickingCharacter->GetPlayerState<ASTWPlayerState>();
	if (PlayerState)
	{
		PickingCharacter->SetGiftVisibility(true);
		PlayerState->bIsCarryingGift = true;
		bHasGift = false;
		UpdateGiftVisibility();
	}
}


void ASTWGiftCaptureSite::OnRep_UpdateGift()
{
	UpdateGiftVisibility();
}

void ASTWGiftCaptureSite::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASTWGiftCaptureSite, bHasGift);
}

