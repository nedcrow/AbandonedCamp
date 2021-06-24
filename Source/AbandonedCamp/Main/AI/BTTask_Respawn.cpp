// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Respawn.h"
#include "CommonAIController.h"
#include "../Characters/CommonCharacter.h"
#include "../MainGS.h"
#include "../SpawnManager.h"

#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTTask_Respawn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	ACommonAIController* AIC = Cast<ACommonAIController>(OwnerComp.GetAIOwner());
	if (AIC) {
		ACommonCharacter* character = Cast<ACommonCharacter>(AIC->GetPawn());
		AMainGS* GS = Cast< AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
		if (GS) {
			ASpawnManager* SM = GS->GetSpawnManager();
			if (SM) {
				if (character->ActorHasTag(TEXT("Camp"))) { SM->SpawnActorThat(ESpawnableType::Camper, character); }
				else if(character->ActorHasTag(TEXT("Stranger"))) { SM->SpawnActorThat(ESpawnableType::Stranger, character); }
				else {UE_LOG(LogTemp, Warning, TEXT("Error: This actor has no tag(Camp or Stranger).")); }
			}
		}
	}
	return EBTNodeResult::Succeeded;
}