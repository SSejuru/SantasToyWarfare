// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STWPlayerState.h"
#include "GameFramework/GameState.h"
#include "SantasToyWarfare/SantasToyWarfareCharacter.h"
#include "SantasToyWarfare/SantasToyWarfarePlayerController.h"
#include "STWGameState.generated.h"

UENUM(BlueprintType)
enum EMessageType : uint8
{
	EM_GiftCaptured  UMETA(DisplayName = "Gift Captured"),
	EM_GiftReturned  UMETA(DisplayName = "Gift Returned"),
	EM_GiftStolen UMETA(DisplayName = "Gift Stolen")
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTeamScoreUpdate, EPlayerTeam, TeamScoring, int64, NewTeamScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEnded, EPlayerTeam, WinningTeam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGameEventMessage, EMessageType, MessageType, EPlayerTeam, AssociatedTeam, float, MessageDuration);


/**
 * 
 */
UCLASS()
class SANTASTOYWARFARE_API ASTWGameState : public AGameState
{
	GENERATED_BODY()

private:

	UPROPERTY(ReplicatedUsing = "OnRep_BlueTeamScored")
	int64 BlueTeamScore;

	UPROPERTY(ReplicatedUsing = "OnRep_RedTeamScored")
	int64 RedTeamScore;

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

	UPROPERTY(BlueprintAssignable)
	FOnGameEnded OnGameEnded;

	UPROPERTY(BlueprintAssignable)
	FOnGameEventMessage OnGameEventMessage;

	UFUNCTION(BlueprintImplementableEvent, Category = "Character Spawning")
	void NotifyCharacterSpawned(ASantasToyWarfareCharacter* Character);

	UFUNCTION(BlueprintImplementableEvent, Category = "Character Spawning")
	void NotifyCharacterDestroyed(ASantasToyWarfareCharacter* Character);

	UFUNCTION()
	void IncreaseTeamScore(EPlayerTeam Team, int64 score);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_EndGame(EPlayerTeam WinningTeam);

	int64 GetTeamScore(EPlayerTeam Team) const { return Team == ET_Blue ? BlueTeamScore : RedTeamScore; };

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastGameEvent(EMessageType Message, EPlayerTeam AssociatedTeam, float Duration);
};
