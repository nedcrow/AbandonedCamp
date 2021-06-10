// Fill out your copyright notice in the Description page of Project Settings.


#include "StrangerAIController.h"
#include "../Characters/StrangerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void AStrangerAIController::SetNightState(ENightState EState) {
	if (BBComponent) BBComponent->SetValueAsEnum(TEXT("NightState"), (uint8)EState);
}