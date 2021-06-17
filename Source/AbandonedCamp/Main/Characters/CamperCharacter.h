// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonCharacter.h"
#include "CamperCharacter.generated.h"

UCLASS()
class ABANDONEDCAMP_API ACamperCharacter : public ACommonCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACamperCharacter();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UStaticMeshComponent* Weapon;

	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	//class UCapsuleComponent* WeaponCapsule;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UPawnSensingComponent* PawnSensing;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void ProcessSeenPawn(APawn* Pawn);

	// Status
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float CurrentHP = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float MaxHP = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float WalkSpeed = 150.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float RunSpeed = 300.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float Attack = 30.0f;

	// OverlapEvent
	//UFUNCTION()
	//void WeaponOverlap(
	//	class UPrimitiveComponent* OverlappedComp, 
	//	class AActor* OtherActor,
	//	class UPrimitiveComponent* OtherComp,
	//	int32 OtherBodyIndex,
	//	bool bFromSweep,
	//	const FHitResult& SweepResult);

	// OverlapEvent
	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
