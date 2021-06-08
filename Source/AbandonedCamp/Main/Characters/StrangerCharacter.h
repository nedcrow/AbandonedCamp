// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonCharacter.h"
#include "StrangerCharacter.generated.h"

UCLASS()
class ABANDONEDCAMP_API AStrangerCharacter : public ACommonCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AStrangerCharacter();

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
		float CurrentHP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
		float MaxHP = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
		float WalkSpeed = 50.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
		float RunSpeed = 300.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
		float WarmthSight = 1000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
		float Attack = 30.0f;
};
