// Fill out your copyright notice in the Description page of Project Settings.


#include "CamperAIController.h"
#include "../Characters/CamperCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void ACamperAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ACamperCharacter* camper = Cast<ACamperCharacter>(InPawn);
	if (camper) {
		SetAvoidanceLocation(camper->GetActorLocation());
		SetTargetLocation(camper->GetActorLocation());
	}
}

void ACamperAIController::SetTargetActor(AActor* Target)
{
	if (BBComponent) BBComponent->SetValueAsObject(TEXT("TargetActor"), Target);
}

void ACamperAIController::SetTargetLocation(FVector Location)
{
	if (BBComponent) BBComponent->SetValueAsVector(TEXT("TargetLocation"), Location);
}

void ACamperAIController::SetAvoidanceLocation(FVector Location)
{
	if (BBComponent) BBComponent->SetValueAsVector(TEXT("AvoidanceLocation"), Location);
}