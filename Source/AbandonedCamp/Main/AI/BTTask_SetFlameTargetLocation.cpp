// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetFlameTargetLocation.h"
#include "../BuildingManager.h"
#include "StrangerAIController.h"

#include "BehaviorTree/BlackBoardComponent.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTTask_SetFlameTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AStrangerAIController* AIC = Cast<AStrangerAIController>(OwnerComp.GetAIOwner());
	if (AIC) {
		APawn* pawn = AIC->GetPawn();
		ABuildingManager* buildingM = Cast<ABuildingManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABuildingManager::StaticClass()));
		bool isAble = pawn && buildingM && buildingM->FireBuildingArr.Num() > 0;

		if (isAble) {
			if (!bUseAvoidance) {
				TArray<FDistanceInfo> DistInfoArr;
				for (auto fire : buildingM->FireBuildingArr) {
				FDistanceInfo tempInfo;
				tempInfo.Actor = fire;
				tempInfo.Distance = FVector::Distance(pawn->GetActorLocation(), fire->GetActorLocation());
					DistInfoArr.Add(tempInfo);
				}
				DistInfoArr.Sort([](FDistanceInfo a, FDistanceInfo b) {
					return a.Distance < b.Distance;
				});
				AIC->CurrentEnermy = DistInfoArr[0].Actor;
				AIC->SetTargetActor(DistInfoArr[0].Actor);
				AIC->SetTargetLocation(DistInfoArr[0].Actor->GetActorLocation());
			}
			else {
				// 저장해둔 타겟 위치로 수정
				AIC->SetTargetLocation(AIC->BBComponent->GetValueAsVector(TEXT("AvoidanceLocation")));
			}
		}
	}

	return EBTNodeResult::Succeeded;
}