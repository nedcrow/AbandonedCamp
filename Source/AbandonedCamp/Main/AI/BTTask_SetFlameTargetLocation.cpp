// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetFlameTargetLocation.h"
#include "../BuildingManager.h"
#include "StrangerAIController.h"

#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTTask_SetFlameTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AStrangerAIController* AIC = Cast<AStrangerAIController>(OwnerComp.GetAIOwner());
	if (AIC) {
		APawn* pawn = AIC->GetPawn();
		ABuildingManager* buildingM = Cast<ABuildingManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABuildingManager::StaticClass()));
		bool isAble = pawn && buildingM && buildingM->FireBuildingArr.Num() > 0;

		if (isAble) {
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
			AIC->SetTargetActor(DistInfoArr[0].Actor);
			AIC->SetTargetLocation(DistInfoArr[0].Actor->GetActorLocation());
		}
	}

	return EBTNodeResult::Succeeded;
}