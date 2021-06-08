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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBehaviorTreeComponent* BTComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBlackboardComponent* BBComponent;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	void SetCurrentState(ECharacterState EState);
};
