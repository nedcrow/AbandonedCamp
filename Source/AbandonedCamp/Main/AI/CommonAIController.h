// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "../Characters/CommonCharacter.h"
#include "CommonAIController.generated.h"

UENUM(BlueprintType)
enum class EMoveState : uint8 {
	ToTarget = 0	UMETA(Display = "ToTarget"),
	ToAvoidance = 1	UMETA(Display = "ToAvoidance"),
};

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

	void SetTargetActor(AActor* Target);

	void SetTargetLocation(FVector Location);

};
