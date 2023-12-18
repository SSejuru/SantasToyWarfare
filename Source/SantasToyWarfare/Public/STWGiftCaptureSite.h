// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STWPlayerState.h"
#include "GameFramework/Actor.h"
#include "SantasToyWarfare/SantasToyWarfareCharacter.h"
#include "STWGiftCaptureSite.generated.h"

class USphereComponent;
class UBoxComponent;

UCLASS()
class SANTASTOYWARFARE_API ASTWGiftCaptureSite : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTWGiftCaptureSite();

	UFUNCTION(Category = "Gameplay")
	void RestockGift();

	UFUNCTION()
	EPlayerTeam GetTeamOwner() const { return TeamOwner.GetValue(); }

private:

	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Box Collision to detect if gift was captured*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> BoxComp;

	/* Sphere collision for detecting collision between characters and the gift */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> TreeStaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> GiftStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	TEnumAsByte<EPlayerTeam> TeamOwner;

	UPROPERTY(ReplicatedUsing= "OnRep_UpdateGift")
	bool bHasGift;

	UFUNCTION()
	void OnRep_UpdateGift();

	UFUNCTION()
	void OnSiteBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnGiftBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Reliable, Server, Category = "Gameplay")
	void Server_CaptureGift(ASantasToyWarfareCharacter* CapturingCharacter);

	UFUNCTION(Reliable, Server, Category = "Gameplay")
	void Server_PickUpGift(ASantasToyWarfareCharacter* PickingCharacter);

	UFUNCTION()
	void UpdateGiftVisibility();
	
};
