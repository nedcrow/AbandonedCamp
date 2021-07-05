// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetPatrolTargetLocation.h"
#include "CamperAIController.h"
#include "../Characters/CamperCharacter.h"
#include "../BuildingManager.h"

#include "BehaviorTree/BlackBoardComponent.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTTask_SetPatrolTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	ACamperAIController* AIC = Cast<ACamperAIController>(OwnerComp.GetAIOwner());
	if (AIC) {
		ACamperCharacter* camper = AIC->GetPawn<ACamperCharacter>();
		if (camper) {
			if (!bUseAvoidance) {
				// Ÿ�� �ֺ� Nm ������ ���� ��ġ ����
				ABuildingManager* buildingM = ABuildingManager::GetInstance();
				if (buildingM->FireBuildingArr.Num() > 0) {
					int max = buildingM->FireBuildingArr.Num() - 1;
					int index = FMath::RandRange(0, max);
					FVector targetLocation = buildingM->FireBuildingArr[index]->GetActorLocation();
					targetLocation = FVector(
						targetLocation.X + (FMath::RandRange(100, 300) * (FMath::RandBool() ? -1 : 1)),
						targetLocation.Y + (FMath::RandRange(100, 300) * (FMath::RandBool() ? -1 : 1)),
						camper->GetActorLocation().Z
					);

					AIC->SetTargetActor(buildingM->FireBuildingArr[index]);
					AIC->SetTargetLocation(targetLocation);
				}
			}
			else {
				// �����ص� Ÿ�� ��ġ�� ����
				AIC->SetTargetLocation(AIC->BBComponent->GetValueAsVector(TEXT("AvoidanceLocation")));
			}

		}
	}
	return EBTNodeResult::Succeeded;
}