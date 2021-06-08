// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonAIController.h"
#include "CamperAIController.generated.h"

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API ACamperAIController : public ACommonAIController
{
	GENERATED_BODY()

public:

	class AActor* CurrentEnermy;

	virtual void OnPossess(APawn* InPawn) override;

	void SetTargetActor(AActor* Target);

	void SetTargetLocation(FVector Location);

	void SetAvoidanceLocation(FVector Location);
};
