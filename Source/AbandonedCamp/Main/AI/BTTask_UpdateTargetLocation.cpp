// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_UpdateTargetLocation.h"
#include "CommonAIController.h"
#include "BehaviorTree/BlackBoardComponent.h"

EBTNodeResult::Type UBTTask_UpdateTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACommonAIController* AIC = Cast<ACommonAIController>(OwnerComp.GetAIOwner());
	if (AIC) {
		AActor* targetActor = Cast<AActor>(AIC->BBComponent->GetValueAsObject(TEXT("TargetActor")));
		FVector targetLocation = AIC->BBComponent->GetValueAsVector(TEXT("TargetLocation"));
		if (targetActor->GetActorLocation() != targetLocation) {
			AIC->SetTargetLocation(targetActor->GetActorLocation());
		}
	}
	return EBTNodeResult::Succeeded;
}
