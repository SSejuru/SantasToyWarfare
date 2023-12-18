// Fill out your copyright notice in the Description page of Project Settings.


#include "STWPlayerState.h"

#include "Net/UnrealNetwork.h"

void ASTWPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASTWPlayerState, AssignedTeam);
	DOREPLIFETIME(ASTWPlayerState, bIsCarryingGift);
}
