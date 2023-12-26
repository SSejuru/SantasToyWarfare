// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STWPlayerState.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "STWGameplayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SANTASTOYWARFARE_API USTWGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static EPlayerTeam GetOppositeTeam(EPlayerTeam Team);

};
