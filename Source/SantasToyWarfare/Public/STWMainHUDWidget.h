// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STWMainHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class SANTASTOYWARFARE_API USTWMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void RestartWidget();
};
