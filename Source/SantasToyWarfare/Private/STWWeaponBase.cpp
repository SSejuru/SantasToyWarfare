// Fill out your copyright notice in the Description page of Project Settings.


#include "STWWeaponBase.h"

#include "SantasToyWarfare/SantasToyWarfareCharacter.h"
#include "SantasToyWarfare/TP_WeaponComponent.h"

// Sets default values
ASTWWeaponBase::ASTWWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponComponent = CreateDefaultSubobject<UTP_WeaponComponent>("WeaponComp");
	RootComponent = WeaponComponent;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComp");
	MeshComponent->SetupAttachment(WeaponComponent);

}

// Called when the game starts or when spawned
void ASTWWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	ASantasToyWarfareCharacter* Character = Cast<ASantasToyWarfareCharacter>(GetOwner());

	if(Character)
	{
		WeaponComponent->AttachWeapon(Character);
	}
}

// Called every frame
void ASTWWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

