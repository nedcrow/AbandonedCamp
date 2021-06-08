// Fill out your copyright notice in the Description page of Project Settings.


#include "StrangerAIController.h"
#include "../Characters/StrangerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void AStrangerAIController::SetTargetActor(AActor* Target)
{
	if (BBComponent) BBComponent->SetValueAsObject(TEXT("TargetActor"), Target);
}

void AStrangerAIController::SetTargetLocation(FVector Location)
{
	if(BBComponent) BBComponent->SetValueAsVector(TEXT("TargetLocation"), Location);
}
