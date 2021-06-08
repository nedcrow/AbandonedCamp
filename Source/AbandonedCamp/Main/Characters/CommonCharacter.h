// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CommonCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle	UMETA(DisplayName = "Idle"),
	ToFlame	UMETA(DisplayName = "ToFlame"),
	Guard	UMETA(DisplayName = "Guard"),
	Battle	UMETA(DisplayName = "Battle"),
	Dead	UMETA(DisplayName = "Dead")
};

UCLASS()
class ABANDONEDCAMP_API ACommonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACommonCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// AI
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	ECharacterState CurrentState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim")
	int CountAttackAnim = 3;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim")
	int CountHitAnim = 3;

	UFUNCTION(BlueprintCallable)
	void SetCurrentState(ECharacterState NewState);

};
