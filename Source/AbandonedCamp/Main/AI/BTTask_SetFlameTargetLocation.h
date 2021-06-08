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
//};
//public:
//	FORCEINLINE FDistanceInfo(AActor* InActor, float InDistance);
	//public:
	//
	//	void Set(AActor* InActor, float InDistance);


//public:
//	FORCEINLINE FDistanceInfo::FDistanceInfo(AActor* InActor, float InDistance)
//		: Actor(InActor), Distance(InDistance)
//	{}
	/*FORCEINLINE void FDistanceInfo::Set(AActor* InActor, float InDistance)
	{
		Actor = InActor;
		Distance = InDistance;
	}*/
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
};
