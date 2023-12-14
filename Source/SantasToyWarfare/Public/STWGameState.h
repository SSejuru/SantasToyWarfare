// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STWPlayerState.h"
#include "GameFramework/GameState.h"
#include "SantasToyWarfare/SantasToyWarfareCharacter.h"
#include "SantasToyWarfare/SantasToyWarfarePlayerController.h"
#include "STWGameState.generated.h"

/**
 * 
 */
UCLASS()
class SANTASTOYWARFARE_API ASTWGameState : public AGameState
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EPlayerTeam GetPlayerTeam(ASantasToyWarfarePlayerController* PlayerController);

	UPROPERTY(BlueprintReadWrite, Category = "PlayerTeams", Replicated)
	TArray<ASantasToyWarfarePlayerController*> BlueTeamPlayers;

	UPROPERTY(BlueprintReadWrite, Category = "PlayerTeams", Replicated)
	TArray<ASantasToyWarfarePlayerController*> RedTeamPlayers;

	UFUNCTION(BlueprintImplementableEvent, Category = "Character Spawning")
	void NotifyCharacterSpawned(ASantasToyWarfareCharacter* Character);

	UFUNCTION(BlueprintImplementableEvent, Category = "Character Spawning")
	void NotifyCharacterDestroyed(ASantasToyWarfareCharacter* Character);
};
