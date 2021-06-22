// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileBase.h"
#include "BuildingTile.generated.h"

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API ABuildingTile : public ATileBase
{
	GENERATED_BODY()
	
public:
	ABuildingTile();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UHUDSceneComponent* HUDScene;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UWidgetComponent* Widget;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Status
	UPROPERTY(ReplicatedUsing = "OnRep_CurrentHP", BlueprintReadWrite, EditAnywhere, Category = "Status")
	float CurrentHP = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float MaxHP = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float AttackPoint = 30.0f;

	// Interaction
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	uint8 bCanInteraction : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	uint8 bCanStartRound : 1;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	uint8 bIsTouched : 1;

	UFUNCTION()
	void CallDelFunc_TouchEvent(FName TargetName, FVector TargetLocation);

	// TakeDamage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION()
	void OnRep_CurrentHP();


	// Effect
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Effects")
	TArray<UParticleSystem*> InteractionEffects;
	TArray<UParticleSystemComponent*> InteractionEffectsComponents;

	void SpawnEffects();
	void DestroyEffects();
};
