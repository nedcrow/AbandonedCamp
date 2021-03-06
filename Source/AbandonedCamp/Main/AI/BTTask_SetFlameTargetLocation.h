// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetFlameTargetLocation.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FDistanceInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Distance;
};

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API UBTTask_SetFlameTargetLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Condition")
	uint8 bUseAvoidance : 1;
};
