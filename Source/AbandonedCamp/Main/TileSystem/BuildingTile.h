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

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UBoxComponent* Box;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UStaticMeshComponent* BodyMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UHUDSceneComponent* HUDScene;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UWidgetComponent* Widget;

protected:
	virtual void PostRegisterAllComponents() override;

public:
	// Interaction
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	uint8 bCanInteraction : 1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	uint8 bIsTouched : 1;

	// Effect
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UParticleSystem*> InteractionEffects;
	TArray<UParticleSystemComponent*> InteractionEffectsComponents;
	UParticleSystemComponent* InteractionEffectsComponent;

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticast_SpawnEffects();
	void NetMulticast_SpawnEffects_Implementation();

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticast_DestroyEffects();
	void NetMulticast_DestroyEffects_Implementation();
};
