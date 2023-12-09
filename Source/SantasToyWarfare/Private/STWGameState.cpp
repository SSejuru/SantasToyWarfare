// Fill out your copyright notice in the Description page of Project Settings.


#include "STWGameState.h"

EPlayerTeam ASTWGameState::GetPlayerTeam(ASantasToyWarfarePlayerController* PlayerController)
{
	ASTWPlayerState* PS =  Cast<ASTWPlayerState>(PlayerController->PlayerState);

	if(PS)
	{
		return PS->AssignedTeam;
	}

	return EPlayerTeam();
}
