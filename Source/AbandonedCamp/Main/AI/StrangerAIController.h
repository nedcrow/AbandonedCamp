// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonAIController.h"
#include "../MainGS.h"
#include "StrangerAIController.generated.h"

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API AStrangerAIController : public ACommonAIController
{
	GENERATED_BODY()
public:

	class AActor* CurrentEnermy;

	void SetTargetActor(AActor* Target);

	void SetTargetLocation(FVector Location);

	void SetNightState(ENightState EState);
};
