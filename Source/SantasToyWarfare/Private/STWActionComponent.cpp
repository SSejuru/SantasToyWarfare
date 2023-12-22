// Fill out your copyright notice in the Description page of Project Settings.


#include "STWActionComponent.h"

#include "Net/UnrealNetwork.h"
#include "Actions/STWAction.h"
#include "Engine/ActorChannel.h"

USTWActionComponent::USTWActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bCanUseActions = true;

	SetIsReplicatedByDefault(true);
}


void USTWActionComponent::BeginPlay()
{
	Super::BeginPlay();

	//Server Only
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<USTWAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void USTWActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<USTWAction*> ActionsCopy = Actions;
	for (USTWAction* Action : ActionsCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}

	Super::EndPlay(EndPlayReason);
}


void USTWActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USTWActionComponent::AddAction(AActor* Instigator, TSubclassOf<USTWAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction. [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}

	USTWAction* NewAction = NewObject<USTWAction>(GetOwner(), ActionClass);

	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void USTWActionComponent::RemoveAction(USTWAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);
}

bool USTWActionComponent::StartActionByTag(AActor* Instigator, FGameplayTag ActionTag)
{
	for (USTWAction* Action : Actions)
	{
		if (Action && Action->ActionTag == ActionTag)
		{
			if (!Action->CanStart(Instigator))
			{
				continue;
			}

			//Is Client?
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionTag);
			}

			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool USTWActionComponent::StopActionByTag(AActor* Instigator, FGameplayTag ActionTag)
{
	for (USTWAction* Action : Actions)
	{
		if (Action && Action->ActionTag == ActionTag)
		{
			if (Action->IsRunning())
			{
				//Is Client?
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionTag);
				}

				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

void USTWActionComponent::ServerStartAction_Implementation(AActor* Instigator, FGameplayTag ActionTag)
{
	StartActionByTag(Instigator, ActionTag);
}

void USTWActionComponent::ServerStopAction_Implementation(AActor* Instigator, FGameplayTag ActionTag)
{
	StopActionByTag(Instigator, ActionTag);
}

void USTWActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USTWActionComponent, Actions);
}

bool USTWActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (USTWAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

