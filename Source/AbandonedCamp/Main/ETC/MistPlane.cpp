// Fill out your copyright notice in the Description page of Project Settings.


#include "MistPlane.h"
#include "../BuildingManager.h"
#include "../SpawnManager.h"
#include "../TileSystem/BonFireComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetRenderingLibrary.h"

// Sets default values
AMistPlane::AMistPlane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMistPlane::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), RenderTarget);
}

void AMistPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentTime < 1) {
		CurrentTime += GetWorld()->GetDeltaSeconds() / SecondPerOneHit;
	}
	else {
		CurrentTime = 0.f;
		DamageOverTime();
	}
}

void AMistPlane::DamageOverTime()
{
	ASpawnManager* SM = ASpawnManager::GetInstance();
	ABuildingManager* BM = ABuildingManager::GetInstance();
	for (auto camper : SM->SpawnedCamperActors) {
		if (BM->FireBuildingArr.Num() == 0) {
			if (GetWorld()->IsServer()) {
				UGameplayStatics::ApplyPointDamage(camper, AttackPoint, FVector(), FHitResult(), GetController(), this, UDamageType::StaticClass());
			}
		}
		else {
			// 여러 캠프빌딩 중 하나라도 벗어나지 않으면 false
			bool isOutOfLight = true;
			for (auto building : BM->FireBuildingArr) {
				float dist = FVector::Distance(
					FVector(camper->GetActorLocation().X, camper->GetActorLocation().Y, 0.f),
					FVector(building->GetActorLocation().X, building->GetActorLocation().Y, 0.f)
				);
				UBonFireComponent* fireComponent = Cast<UBonFireComponent>(building->GetComponentByClass(UBonFireComponent::StaticClass()));
				bool isInOfLight = fireComponent && dist <= (fireComponent->FireLightRadius + 1.f) * 100;
				if (isInOfLight) {
					isOutOfLight = false;
					goto EndOutOfLightChecking;
				}
			}
			EndOutOfLightChecking:
			if (isOutOfLight) {
				if (GetWorld()->IsServer()) {
					UGameplayStatics::ApplyPointDamage(camper, AttackPoint, FVector(), FHitResult(), GetController(), this, UDamageType::StaticClass());
				}
			}
		}		
	}
}
