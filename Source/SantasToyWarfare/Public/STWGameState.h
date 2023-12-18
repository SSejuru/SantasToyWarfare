// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STWPlayerState.h"
#include "GameFramework/GameState.h"
#include "SantasToyWarfare/SantasToyWarfareCharacter.h"
#include "SantasToyWarfare/SantasToyWarfarePlayerController.h"
#include "STWGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTeamScoreUpdate, EPlayerTeam, TeamScoring, int16, NewTeamScore);

/**
 * 
 */
UCLASS()
class SANTASTOYWARFARE_API ASTWGameState : public AGameState
{
	GENERATED_BODY()

private:

	UPROPERTY(ReplicatedUsing = "OnRep_BlueTeamScored")
	int16 BlueTeamScore;

	UPROPERTY(ReplicatedUsing = "OnRep_RedTeamScored")
	int16 RedTeamScore;

	UFUNCTION()
	void OnRep_BlueTeamScored();

	UFUNCTION()
	void OnRep_RedTeamScored();

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EPlayerTeam GetPlayerTeam(ASantasToyWarfarePlayerController* PlayerController);

	UPROPERTY(BlueprintReadWrite, Category = "PlayerTeams", Replicated)
	TArray<ASantasToyWarfarePlayerController*> BlueTeamPlayers;

	UPROPERTY(BlueprintReadWrite, Category = "PlayerTeams", Replicated)
	TArray<ASantasToyWarfarePlayerController*> RedTeamPlayers;

	UPROPERTY(BlueprintAssignable)
	FOnTeamScoreUpdate OnTeamScoreUpdate;

	UFUNCTION(BlueprintImplementableEvent, Category = "Character Spawning")
	void NotifyCharacterSpawned(ASantasToyWarfareCharacter* Character);

	UFUNCTION(BlueprintImplementableEvent, Category = "Character Spawning")
	void NotifyCharacterDestroyed(ASantasToyWarfareCharacter* Character);

	UFUNCTION()
	void IncreaseTeamScore(EPlayerTeam Team, int16 score);
};
