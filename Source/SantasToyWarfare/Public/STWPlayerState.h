// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "STWPlayerState.generated.h"

UENUM(BlueprintType)
enum EPlayerTeam : uint8
{
	ET_Blue  UMETA(DisplayName = "Blue Team"),
	ET_Red  UMETA(DisplayName = "Red Team"),
};

/**
 * 
 */
UCLASS()
class SANTASTOYWARFARE_API ASTWPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = "Gameplay", Replicated)
	TEnumAsByte<EPlayerTeam> AssignedTeam;

	UPROPERTY(BlueprintReadWrite, Category = "Gameplay", Replicated)
	bool bIsCarryingGift;
};
