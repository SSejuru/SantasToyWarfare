// Fill out your copyright notice in the Description page of Project Settings.


#include "STWAttributesComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USTWAttributesComponent::USTWAttributesComponent()
{
	MaxHealth = 100;
	Health = 100;

	SetIsReplicatedByDefault(true);
}


void USTWAttributesComponent::MultiCastHealthChange_Implementation(AActor* InstigatorActor, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta);
}

bool USTWAttributesComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f) { return false; }

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	float ActualDelta = NewHealth - OldHealth;

	if(GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if(ActualDelta != 0)
		{
			MultiCastHealthChange(InstigatorActor, ActualDelta);
		}

		if(ActualDelta < 0.0f && Health == 0)
		{
			//Set up player death
		}
	}

	return ActualDelta != 0;
}

USTWAttributesComponent* USTWAttributesComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USTWAttributesComponent>(FromActor->GetComponentByClass(USTWAttributesComponent::StaticClass()));
	}

	return nullptr;
}

bool USTWAttributesComponent::IsActorAlive(AActor* Actor)
{
	USTWAttributesComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

void USTWAttributesComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USTWAttributesComponent, Health);
	DOREPLIFETIME(USTWAttributesComponent, MaxHealth);

	//DOREPLIFETIME_CONDITION(USAttributeComponent, MaxHealth, COND_OwnerOnly);
}

