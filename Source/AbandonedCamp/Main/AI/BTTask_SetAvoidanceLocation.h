// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetAvoidanceLocation.generated.h"


UENUM(BlueprintType)
enum class EDirection : uint8
{
	Center	UMETA(DisplayName = "Center"),
	Left	UMETA(DisplayName = "Left"),
	Right	UMETA(DisplayName = "Right"),
};

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API UBTTask_SetAvoidanceLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Condition", meta = (ClampMin = "5", ClampMax = "91", UIMin = "5", UIMax = "91"))
	int CountOfTracePerHalfround = 5;

	EDirection LastTurn;

	FVector FindNoBlocking(APawn* Pawn, FVector AvoidanceLocation, EDirection SearchDir = EDirection::Center);
};
