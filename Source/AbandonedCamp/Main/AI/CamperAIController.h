// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "../Characters/CamperCharacter.h"
#include "CamperAIController.generated.h"

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API ACamperAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACamperAIController();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBehaviorTreeComponent* BTComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBlackboardComponent* BBComponent;

	class AActor* CurrentEnermy;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	void SetCurrentState(ECharacterState EState);

	void SetTargetActor(AActor* Target);

	void SetTargetLocation(FVector Location);

	void SetAvoidanceLocation(FVector Location);
};
