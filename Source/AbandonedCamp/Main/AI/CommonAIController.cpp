// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonAIController.h"
#include "../Characters/CommonCharacter.h" 
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
		WalkSpeed = character->GetCharacterMovement()->GetMaxSpeed();
	}
}

void ACommonAIController::OnUnPossess()
{
	BTComponent->StopTree();
	Super::OnUnPossess();
}

void ACommonAIController::SetCurrentState(ECharacterState EState)
{
	ACommonCharacter* character = Cast<ACommonCharacter>(GetPawn());
	if (character && EState != ECharacterState::Guard) character->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	if (BBComponent) BBComponent->SetValueAsEnum(TEXT("CurrentState"), (uint8)EState);
}

void ACommonAIController::SetTargetActor(AActor* Target)
{
	CurrentEnermy = Target;
	if (BBComponent) BBComponent->SetValueAsObject(TEXT("TargetActor"), Target);
}

void ACommonAIController::SetTargetLocation(FVector Location)
{
	if (BBComponent) BBComponent->SetValueAsVector(TEXT("TargetLocation"), Location);
}
