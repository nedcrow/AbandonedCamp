// Fill out your copyright notice in the Description page of Project Settings.


#include "CamperAIController.h"
#include "../Characters/CamperCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

ACamperAIController::ACamperAIController() {
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComponent"));
}

void ACamperAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ACamperCharacter* camper = Cast<ACamperCharacter>(InPawn);
	if (camper) {
		if (camper->BehaviorTree && camper->BehaviorTree->BlackboardAsset) {
			BBComponent->InitializeBlackboard(*(camper->BehaviorTree->BlackboardAsset));
			BTComponent->StartTree(*(camper->BehaviorTree));

			//if (BBComponent)BBComponent->SetValueAsObject(TEXT("SelfActor"), camper);
			if (BBComponent)BBComponent->SetValueAsVector(TEXT("AvoidanceLocation"), camper->GetActorLocation());
			SetTargetLocation(camper->GetActorLocation());
		}
	}
}

void ACamperAIController::OnUnPossess()
{
	BTComponent->StopTree();
	Super::OnUnPossess();
}

void ACamperAIController::SetCurrentState(ECharacterState EState)
{
	if (BBComponent) BBComponent->SetValueAsEnum(TEXT("CurrentState"), (uint8)EState);
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