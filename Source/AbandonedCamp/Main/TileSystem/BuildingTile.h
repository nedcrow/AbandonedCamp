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
	virtual void PostRegisterAllComponents() override;
	virtual void BeginPlay() override;

public:
	// Interaction
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	uint8 bCanInteraction : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	uint8 bCanStartRound : 1;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	uint8 bIsTouched : 1;

	UFUNCTION()
	void CallDelFunc_TouchEvent(FName TargetName, FVector TargetLocation);


	// Effect
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Effects")
	TArray<UParticleSystem*> InteractionEffects;
	TArray<UParticleSystemComponent*> InteractionEffectsComponents;

	void SpawnEffects();
	void DestroyEffects();
};
