// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STWPlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "STWPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class SANTASTOYWARFARE_API ASTWPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EPlayerTeam> AssignedTeam;
	
};
