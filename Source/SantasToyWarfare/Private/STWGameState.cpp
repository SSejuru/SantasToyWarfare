// Fill out your copyright notice in the Description page of Project Settings.


#include "STWGameState.h"

#include "Net/UnrealNetwork.h"

EPlayerTeam ASTWGameState::GetPlayerTeam(ASantasToyWarfarePlayerController* PlayerController)
{
	ASTWPlayerState* PS =  Cast<ASTWPlayerState>(PlayerController->PlayerState);

	if(PS)
	{
		return PS->AssignedTeam;
	}

	return EPlayerTeam();
}

void ASTWGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASTWGameState, BlueTeamPlayers);
	DOREPLIFETIME(ASTWGameState, RedTeamPlayers);
}
