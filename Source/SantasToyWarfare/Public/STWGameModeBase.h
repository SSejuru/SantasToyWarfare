// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STWGameModeBase.generated.h"


UENUM(BlueprintType)
enum EPlayerTeam : uint8
{
	TE_BlueTeam UMETA(DisplayName = "Blue Team"),
	TE_RedTeam UMETA(DisplayName = "Red Team")
};

/**
 * 
 */
UCLASS()
class SANTASTOYWARFARE_API ASTWGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
};
