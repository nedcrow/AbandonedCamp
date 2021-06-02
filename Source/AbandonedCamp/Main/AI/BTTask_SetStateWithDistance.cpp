// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetStateWithDistance.h"
#include "CamperAIController.h"
#include "../Characters/CamperCharacter.h"

#include "BehaviorTree/BlackBoardComponent.h"

EBTNodeResult::Type UBTTask_SetStateWithDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	ACamperAIController* AIC = Cast<ACamperAIController>(OwnerComp.GetAIOwner());
	if(AIC){
		ACamperCharacter* camper = AIC->GetPawn<ACamperCharacter>();
		FVector targetLocation = AIC->BBComponent->GetValueAsVector(TEXT("TargetLocation"));

		if (camper)
		{
			if (camper->CurrentState != ECharacterState::Dead) {
				float CurrentDistance = FVector::Distance(
					camper->GetActorLocation(),
					targetLocation
				);

				UE_LOG(LogTemp, Warning, TEXT("TargetLocation : %f, %f, %f"), targetLocation.X, targetLocation.Y, targetLocation.Z);
				UE_LOG(LogTemp, Warning, TEXT("dist : %f"), CurrentDistance);
				switch (TargetCondition)
				{
				case ECondition::LessThenDistance: // <

					if (CurrentDistance <= TargetDistance)
					{
						camper->SetCurrentState(TargetState);
					}
					break;

				case ECondition::GreaterThenDistance: // >

					if (CurrentDistance >= TargetDistance)
					{
						camper->SetCurrentState(TargetState);
					}
					break;
				case ECondition::IgnoreDistance:
					camper->SetCurrentState(TargetState);					
					break;
				}			
			}
		}
	}
	return EBTNodeResult::Succeeded;
}
