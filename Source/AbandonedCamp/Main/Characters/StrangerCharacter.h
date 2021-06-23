// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonCharacter.h"
#include "../MainGS.h"
#include "StrangerCharacter.generated.h"

UCLASS()
class ABANDONEDCAMP_API AStrangerCharacter : public ACommonCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AStrangerCharacter();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class USphereComponent* WeaponA;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class USphereComponent* WeaponB;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UStaticMeshComponent* WeaponC;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UHUDSceneComponent* HUDScene;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UWidgetComponent* HPBarWidget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UPawnSensingComponent* PawnSensing;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:		
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void ProcessSeenPawn(APawn* Pawn);

	// Status
	UPROPERTY(ReplicatedUsing = "OnRep_CurrentHP", BlueprintReadWrite, EditAnywhere, Category = "Status")
		float CurrentHP = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
		float MaxHP = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
		float WalkSpeed = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
		float RunSpeed = 200.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
		float WarmthSight = 1000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
		float AttackPoint = 30.0f;

	UFUNCTION()
		void CallDelFunc_OnNightEvent(ENightState NightState);

	// OverlapEvent
	UFUNCTION()
		void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// TakeDamage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION()
		void OnRep_CurrentHP();
};
