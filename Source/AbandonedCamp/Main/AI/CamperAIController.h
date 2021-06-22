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

	virtual void OnPossess(APawn* InPawn) override;

	void SetAvoidanceLocation(FVector Location);
};
