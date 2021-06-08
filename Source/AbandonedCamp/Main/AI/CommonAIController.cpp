// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonAIController.h"
#include "../Characters/CommonCharacter.h" 
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

ACommonAIController::ACommonAIController() {
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComponent"));
}

void ACommonAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ACommonCharacter* character = Cast<ACommonCharacter>(InPawn);
	if (character) {
		if (character->BehaviorTree && character->BehaviorTree->BlackboardAsset) {
			BBComponent->InitializeBlackboard(*(character->BehaviorTree->BlackboardAsset));
			BTComponent->StartTree(*(character->BehaviorTree));
		}
	}
}

void ACommonAIController::OnUnPossess()
{
	BTComponent->StopTree();
	Super::OnUnPossess();
}

void ACommonAIController::SetCurrentState(ECharacterState EState)
{
	if (BBComponent) BBComponent->SetValueAsEnum(TEXT("CurrentState"), (uint8)EState);
}
