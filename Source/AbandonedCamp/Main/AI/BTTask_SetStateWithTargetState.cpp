// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetStateWithTargetState.h"
#include "CommonAIController.h"
#include "../Characters/CommonCharacter.h"

#include "BehaviorTree/BlackBoardComponent.h"

EBTNodeResult::Type UBTTask_SetStateWithTargetState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	ACommonAIController* AIC = Cast<ACommonAIController>(OwnerComp.GetAIOwner());
	if (AIC) {
		ACommonCharacter* character = AIC->GetPawn<ACommonCharacter>();
		ACommonCharacter* targetCharacter = Cast<ACommonCharacter>(AIC->BBComponent->GetValueAsObject(TEXT("TargetActor")));
		if (character && targetCharacter && targetCharacter->CurrentState == TargetConditionState)
		{
			AIC->CurrentEnermy = nullptr;
			character->SetCurrentState(TargetState);
		}
		else {

		}
	}
	return EBTNodeResult::Succeeded;
}