// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/STWAction.h"

#include "STWActionComponent.h"
#include "Net/UnrealNetwork.h"


void USTWAction::Initialize(USTWActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

void USTWAction::StartAction_Implementation(AActor* Instigator)
{
	USTWActionComponent* Comp = GetOwningComponent();

	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("Trying to start action %s"), *ActionTag.ToString()));

	Comp->ActiveGameplayTags.AppendTags(GrantsTag);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
		TimeStarted = GetWorld()->TimeSeconds;

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);

}

void USTWAction::StopAction_Implementation(AActor* Instigator)
{
	USTWActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.RemoveTags(GrantsTag);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}


bool USTWAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning()) return false;

	USTWActionComponent* Comp = GetOwningComponent();

	if (Comp->ActiveGameplayTags.HasAny(BlockedTags) || !Comp->bCanUseActions)
	{
		return false;
	}

	return true;
}


UWorld* USTWAction::GetWorld() const
{
	//Outer is set when creating action via NewObject<T>
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

USTWActionComponent* USTWAction::GetOwningComponent() const
{
	return ActionComp;
}


void USTWAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}


bool USTWAction::IsRunning() const
{
	return RepData.bIsRunning;
}


void USTWAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USTWAction, RepData);
	DOREPLIFETIME(USTWAction, ActionComp);
	DOREPLIFETIME(USTWAction, TimeStarted);
}