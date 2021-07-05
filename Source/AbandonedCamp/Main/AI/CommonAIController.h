// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "../Characters/CommonCharacter.h"
#include "CommonAIController.generated.h"

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API ACommonAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACommonAIController();

public:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

// Property
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBehaviorTreeComponent* BTComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBlackboardComponent* BBComponent;

// Temp data
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class AActor* CurrentEnermy;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float WalkSpeed;

// Set blackboard
public:
	void SetCurrentState(ECharacterState EState);

	void SetTargetActor(AActor* Target);

	void SetTargetLocation(FVector Location);

};
