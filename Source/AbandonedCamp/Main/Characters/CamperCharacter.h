// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CamperCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle	UMETA(DisplayName = "Idle"),
	Guard	UMETA(DisplayName = "Guard"),
	Run		UMETA(DisplayName = "Run"),
	Battle	UMETA(DisplayName = "Battle"),
	Dead	UMETA(DisplayName = "Dead")
};

UCLASS()
class ABANDONEDCAMP_API ACamperCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACamperCharacter();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UStaticMeshComponent* Weapon;

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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// AI
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = "OnRep_CurrentState", Category = "AI")
	ECharacterState CurrentState;

	UFUNCTION()
	void OnRep_CurrentState();

	UFUNCTION(BlueprintCallable)
	void SetCurrentState(ECharacterState NewState);

	UFUNCTION()
	void ProcessSeenPawn(APawn* Pawn);

	// Status
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float CurrentHP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float MaxHP = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float WalkSpeed = 150.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float RunSpeed = 300.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float Attack = 30.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float AttackRange = 100.0f;
};
