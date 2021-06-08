// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "../Characters/CommonCharacter.h"
#include "BTTask_SetStateWithDistance.generated.h"

UENUM(BlueprintType)
enum class ECondition : uint8 {
	LessThenDistance = 0	UMETA(Display = "<= TargetDistance"),
	GreaterThenDistance = 1	UMETA(Display = ">= TargetDistance"),
	IgnoreDistance = 2		UMETA(Display = "IgnoreDistance"),
};

UCLASS()
class ABANDONEDCAMP_API UBTTask_SetStateWithDistance : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Condition")
	ECondition TargetCondition;

	/*Centimeter unit*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Condition")
	float TargetDistance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Condition")
	ECharacterState TargetState;
};
