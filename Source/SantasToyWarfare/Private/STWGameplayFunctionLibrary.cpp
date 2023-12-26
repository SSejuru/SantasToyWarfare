// Fill out your copyright notice in the Description page of Project Settings.


#include "STWGameplayFunctionLibrary.h"

EPlayerTeam USTWGameplayFunctionLibrary::GetOppositeTeam(EPlayerTeam Team)
{
	return Team == ET_Blue ? ET_Red : ET_Red;
}
