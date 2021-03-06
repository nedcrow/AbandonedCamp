// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetStateWithDistance.h"
#include "CommonAIController.h"
#include "../Characters/CommonCharacter.h"

#include "BehaviorTree/BlackBoardComponent.h"

EBTNodeResult::Type UBTTask_SetStateWithDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	ACommonAIController* AIC = Cast<ACommonAIController>(OwnerComp.GetAIOwner());
	if(AIC){
		ACommonCharacter* character = AIC->GetPawn<ACommonCharacter>();
		FVector targetLocation = AIC->BBComponent->GetValueAsVector(TEXT("TargetLocation"));
		AActor* targetActor = Cast<AActor>(AIC->BBComponent->GetValueAsObject(TEXT("TargetActor")));

		if (character)
		{
			if (character->CurrentState != ECharacterState::Dead) {
				float CurrentDistance = FVector::Distance(
					character->GetActorLocation(),
					targetLocation
				);

				switch (TargetCondition)
				{
				case ECondition::LessThenDistance: // <
					if (CurrentDistance <= TargetDistance)
					{
						character->SetCurrentState(TargetState);
					}
					break;

				case ECondition::GreaterThenDistance: // >
					if (CurrentDistance >= TargetDistance)
					{
						character->SetCurrentState(TargetState);
					}
					break;
				case ECondition::IgnoreDistance:
					character->SetCurrentState(TargetState);
					break;
				default:
					break;
				}				
			}

			// 목표가 없으면 Idle로 변경
			if(!targetActor) character->SetCurrentState(ECharacterState::Idle);
		}
	}
	return EBTNodeResult::Succeeded;
}
