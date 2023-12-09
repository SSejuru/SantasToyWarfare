// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STWAttributesComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USTWAttributesComponent*, OwningComp, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SANTASTOYWARFARE_API USTWAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static USTWAttributesComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static bool IsActorAlive(AActor* Actor);

	// Sets default values for this component's properties
	USTWAttributesComponent();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Attributes", Replicated)
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes", Replicated)
	float Health;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastHealthChange(AActor* InstigatorActor, float NewHealth,float Delta);

public:	

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsMaxHealth() const { return Health == MaxHealth; }

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const { return Health > 0; }
};
