// Fill out your copyright notice in the Description page of Project Settings.


#include "STWGameMode.h"

#include "EngineUtils.h"
#include "STWGameplayFunctionLibrary.h"
#include "STWGameState.h"
#include "Net/UnrealNetwork.h"
#include "SantasToyWarfare/SantasToyWarfareCharacter.h"


ASTWGameMode::ASTWGameMode()
{
	RespawnTime = 5.0f;
	PointsOnCapture = 1;
	PointsToWin = 4;
	bCanScorePoints = true;
}

void ASTWGameMode::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ASTWGiftCaptureSite> It(GetWorld()); It; ++It)
	{
		ASTWGiftCaptureSite* GiftCaptureSite = *It;

		switch (GiftCaptureSite->GetTeamOwner())
		{
		case ET_Blue:
			BlueTeamCaptureSite = GiftCaptureSite;
			break;
		case ET_Red:
			RedTeamCaptureSite = GiftCaptureSite;
			break;
		default:
			continue;
		}
	}
}

void ASTWGameMode::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASantasToyWarfareCharacter* Victim = Cast<ASantasToyWarfareCharacter>(VictimActor);

	if (Victim)
	{
		AController* VictimController = Victim->GetController();
		ASTWGameState* GS = GetGameState<ASTWGameState>();


		//Check if victim was carrying gift, and restock it to christmas tree (Change in the future to drop the gift to the ground)
		ASTWPlayerState* VictimPlayerState = VictimController->GetPlayerState<ASTWPlayerState>();
		if (VictimPlayerState && VictimPlayerState->bIsCarryingGift)
		{
			VictimPlayerState->bIsCarryingGift = false;

			EPlayerTeam VictimTeam = VictimPlayerState->AssignedTeam.GetValue();
			EPlayerTeam MessageTeam = ET_Blue;

			switch (VictimTeam)
			{
			case ET_Blue:
				RedTeamCaptureSite->RestockGift();
				MessageTeam = ET_Red;
				break;
			case ET_Red:
				BlueTeamCaptureSite->RestockGift();
				MessageTeam = ET_Blue;
				break;
			}

			if (GS)
			{
				GS->Multicast_BroadcastGameEvent(EM_GiftReturned, MessageTeam, 2.5f);
			}
		}

		//Unpossess character and consider it destroyed
		VictimController->UnPossess();

		if (GS)
		{
			GS->NotifyCharacterDestroyed(Victim);
		}

		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", VictimController);

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnTime, false);
	}
}

void ASTWGameMode::CaptureGift(EPlayerTeam ScoringTeam)
{
	if (bCanScorePoints)
	{
		ASTWGameState* GS = GetGameState<ASTWGameState>();
		if (GS)
		{
			GS->IncreaseTeamScore(ScoringTeam, PointsOnCapture);


			if (GS->GetTeamScore(ScoringTeam) >= PointsToWin)
			{
				bCanScorePoints = false;
				GS->Multicast_EndGame(ScoringTeam);
			}else
			{
				GS->Multicast_BroadcastGameEvent(EM_GiftCaptured, ScoringTeam, 2.5f);
			}
		}
	}

	switch (ScoringTeam)
	{
	case ET_Blue:
		RedTeamCaptureSite->RestockGift();
		break;
	case ET_Red:
		BlueTeamCaptureSite->RestockGift();
		break;
	}
}

void ASTWGameMode::RespawnPlayerElapsed(AController* Controller)
{
	ASantasToyWarfarePlayerController* PC = Cast<ASantasToyWarfarePlayerController>(Controller);

	if (ensure(PC))
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

	if (ensureAlways(PlayerCharacter))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ASantasToyWarfareCharacter* SpawnedCharacter = GetWorld()->SpawnActor<ASantasToyWarfareCharacter>(PlayerCharacter, SpawnerTransform, SpawnParams);
		if (SpawnedCharacter)
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

	for (int i = 0; i < TeamSpawner.Num(); i++)
	{
		if (TeamSpawner[i].PlayerController == nullptr)
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

	for (FPlayerSpawnerData Spawner : TeamSpawner)
	{
		if (Spawner.PlayerController == Player)
		{
			return true;
		}
	}
	return false;
}

TArray<FPlayerSpawnerData> ASTWGameMode::GetTeamSpawner(EPlayerTeam Team)
{
	if (!bSpawnerDataSaved)
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
			break;
		case ET_Red:
			RedTeamSpawnersData.Add(FPlayerSpawnerData(Spawner, nullptr));
			break;
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
		if (Spawner.PlayerController == Player)
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

