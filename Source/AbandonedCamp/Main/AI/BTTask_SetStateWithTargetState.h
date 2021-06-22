// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "../Characters/CommonCharacter.h"
#include "BTTask_SetStateWithTargetState.generated.h"

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API UBTTask_SetStateWithTargetState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Condition")
	ECharacterState TargetConditionState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Condition")
	ECharacterState TargetState;
};
