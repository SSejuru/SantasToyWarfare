// Fill out your copyright notice in the Description page of Project Settings.


#include "STWGameState.h"

#include "Net/UnrealNetwork.h"

void ASTWGameState::OnRep_BlueTeamScored()
{
	OnTeamScoreUpdate.Broadcast(ET_Blue, BlueTeamScore);
}

void ASTWGameState::OnRep_RedTeamScored()
{
	OnTeamScoreUpdate.Broadcast(ET_Red, RedTeamScore);
}

EPlayerTeam ASTWGameState::GetPlayerTeam(ASantasToyWarfarePlayerController* PlayerController)
{
	ASTWPlayerState* PS =  Cast<ASTWPlayerState>(PlayerController->PlayerState);

	if(PS)
	{
		return PS->AssignedTeam;
	}

	return EPlayerTeam();
}

void ASTWGameState::IncreaseTeamScore(EPlayerTeam Team, int64 score)
{
	switch(Team)
	{
		case ET_Blue:
			BlueTeamScore += score;
			OnRep_BlueTeamScored();
			break;

		case ET_Red:
			RedTeamScore += score;
			OnRep_RedTeamScored();
			break;
	}
}

void ASTWGameState::Multicast_EndGame_Implementation(EPlayerTeam WinningTeam)
{
	OnGameEnded.Broadcast(WinningTeam);
}

void ASTWGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASTWGameState, BlueTeamPlayers);
	DOREPLIFETIME(ASTWGameState, RedTeamPlayers);
	DOREPLIFETIME(ASTWGameState, BlueTeamScore);
	DOREPLIFETIME(ASTWGameState, RedTeamScore);
}
