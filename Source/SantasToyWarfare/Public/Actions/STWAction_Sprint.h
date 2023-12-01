// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/STWAction.h"
#include "STWAction_Sprint.generated.h"

/**
 * 
 */
UCLASS()
class SANTASTOYWARFARE_API USTWAction_Sprint : public USTWAction
{
	GENERATED_BODY()

	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

	USTWAction_Sprint();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float RunningSpeed;

};
