// Fill out your copyright notice in the Description page of Project Settings.


#include "STWGameMode.h"

#include "EngineUtils.h"
#include "STWGameState.h"
#include "Net/UnrealNetwork.h"
#include "SantasToyWarfare/SantasToyWarfareCharacter.h"


ASTWGameMode::ASTWGameMode()
{
	RespawnTime = 5.0f;
}

void ASTWGameMode::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASantasToyWarfareCharacter* Victim = Cast<ASantasToyWarfareCharacter>(VictimActor);

	if(Victim)
	{
		AController* VictimController = Victim->GetController();
		VictimController->UnPossess();

		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", VictimController);

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnTime, false);
	}
}

void ASTWGameMode::RespawnPlayerElapsed(AController* Controller)
{
	ASantasToyWarfarePlayerController* PC = Cast<ASantasToyWarfarePlayerController>(Controller);

	if(ensure(PC))
	{
		SpawnPlayer(PC);
	}
}


void ASTWGameMode::SpawnPlayer(ASantasToyWarfarePlayerController* Player)
{
	if (!IsPlayerSpawnAssigned(Player)) { AssignPlayerToSpawner(Player); }

	APawn* PlayerPawn = Player->GetPawn();
	if (PlayerPawn)
	{
		PlayerPawn->Destroy();
	}

	FTransform SpawnerTransform = GetPlayerSpawnTransform(Player);

	if(ensureAlways(PlayerCharacter))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ASantasToyWarfareCharacter* SpawnedCharacter = GetWorld()->SpawnActor<ASantasToyWarfareCharacter>(PlayerCharacter, SpawnerTransform, SpawnParams);
		if(SpawnedCharacter)
		{
			Player->Possess(SpawnedCharacter);
		}

		ASTWGameState* GS = GetGameState<ASTWGameState>();
		if (GS)
		{
			GS->NotifyCharacterSpawned(SpawnedCharacter);
		}
	}
}


void ASTWGameMode::AssignPlayerToSpawner(ASantasToyWarfarePlayerController* Player)
{
	EPlayerTeam PlayerTeam = Player->GetPlayerTeam();
	TArray<FPlayerSpawnerData> TeamSpawner = GetTeamSpawner(PlayerTeam);

	for(int i = 0; i < TeamSpawner.Num(); i++)
	{
		if(TeamSpawner[i].PlayerController == nullptr)
		{
			TeamSpawner[i].PlayerController = Player;
			break;
		}
	}

	//Save Data in Spawners Variable
	if (PlayerTeam == ET_Blue)
		BlueTeamSpawnersData = TeamSpawner;
	else
		RedTeamSpawnersData = TeamSpawner;
}

bool ASTWGameMode::IsPlayerSpawnAssigned(ASantasToyWarfarePlayerController* Player)
{
	TArray<FPlayerSpawnerData> TeamSpawner = GetTeamSpawner(Player->GetPlayerTeam());

	for(FPlayerSpawnerData Spawner : TeamSpawner)
	{
		if(Spawner.PlayerController == Player)
		{
			return true;
		}
	}
	return false;
}

TArray<FPlayerSpawnerData> ASTWGameMode::GetTeamSpawner(EPlayerTeam Team) 
{
	if(!bSpawnerDataSaved)
		SaveTeamSpawners();

	switch (Team)
	{
		case ET_Blue:
			return BlueTeamSpawnersData;
		case ET_Red: 
			return RedTeamSpawnersData;
		default:
			return TArray<FPlayerSpawnerData>();
	}

}

void ASTWGameMode::SaveTeamSpawners()
{
	for (TActorIterator<ASTWPlayerStart> It(GetWorld()); It; ++It)
	{
		ASTWPlayerStart* Spawner = *It;

		switch (Spawner->AssignedTeam)
		{
		case ET_Blue:
			BlueTeamSpawnersData.Add(FPlayerSpawnerData(Spawner, nullptr));
		case ET_Red:
			RedTeamSpawnersData.Add(FPlayerSpawnerData(Spawner, nullptr));
		default:
			continue;
		}
	}

	bSpawnerDataSaved = true;
}

FTransform ASTWGameMode::GetPlayerSpawnTransform(ASantasToyWarfarePlayerController* Player)
{
	TArray<FPlayerSpawnerData> TeamSpawner = GetTeamSpawner(Player->GetPlayerTeam());

	for (FPlayerSpawnerData Spawner : TeamSpawner)
	{
		if(Spawner.PlayerController == Player)
		{
			return Spawner.PlayerStart->GetActorTransform();
		}
	}

	return FTransform();
}


void ASTWGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASTWGameMode, RespawnTime);
}

