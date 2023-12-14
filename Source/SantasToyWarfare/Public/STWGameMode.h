// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STWPlayerStart.h"
#include "GameFramework/GameMode.h"
#include "SantasToyWarfare/SantasToyWarfareCharacter.h"
#include "SantasToyWarfare/SantasToyWarfarePlayerController.h"
#include "STWGameMode.generated.h"

USTRUCT(BlueprintType)
struct FPlayerSpawnerData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASTWPlayerStart* PlayerStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASantasToyWarfarePlayerController* PlayerController;

	FPlayerSpawnerData(ASTWPlayerStart* Spawner, ASantasToyWarfarePlayerController* Player)
	{
		PlayerStart = Spawner;
		PlayerController = Player;
	}

	FPlayerSpawnerData()
	{
		PlayerStart = nullptr;
		PlayerController = nullptr;
	}
};

/**
 * 
 */
UCLASS()
class SANTASTOYWARFARE_API ASTWGameMode : public AGameMode
{
	GENERATED_BODY()

	ASTWGameMode();

private:

	bool bSpawnerDataSaved;

	bool IsPlayerSpawnAssigned(ASantasToyWarfarePlayerController* Player);

	TArray<FPlayerSpawnerData> GetTeamSpawner(EPlayerTeam Team);

	void AssignPlayerToSpawner(ASantasToyWarfarePlayerController* Player);

	void SaveTeamSpawners();

	FTransform GetPlayerSpawnTransform(ASantasToyWarfarePlayerController* Player);

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Spawn Data")
	TArray<FPlayerSpawnerData> BlueTeamSpawnersData;

	UPROPERTY(BlueprintReadWrite, Category = "Spawn Data")
	TArray<FPlayerSpawnerData> RedTeamSpawnersData;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn Data")
	TSubclassOf<ASantasToyWarfareCharacter> PlayerCharacter;

public:

	UFUNCTION(BlueprintCallable, Category = "Player Spawning")
	void SpawnPlayer(ASantasToyWarfarePlayerController* Player);

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Spawn Data")
	float RespawnTime;

	UFUNCTION(BlueprintCallable)
	void OnActorKilled(AActor* VictimActor, AActor* Killer);

	UFUNCTION(BlueprintCallable)
	void RespawnPlayerElapsed(AController* Controller);
};
