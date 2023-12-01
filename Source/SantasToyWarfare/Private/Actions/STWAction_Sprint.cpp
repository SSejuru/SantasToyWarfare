// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/STWAction_Sprint.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "SantasToyWarfare/SantasToyWarfareCharacter.h"


USTWAction_Sprint::USTWAction_Sprint()
{
	WalkSpeed = 600.0f;
	RunningSpeed = 900.0f;
}


void USTWAction_Sprint::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ASantasToyWarfareCharacter* Character = Cast<ASantasToyWarfareCharacter>(Instigator);

	if(Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}

}

void USTWAction_Sprint::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	ASantasToyWarfareCharacter* Character = Cast<ASantasToyWarfareCharacter>(Instigator);

	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}



