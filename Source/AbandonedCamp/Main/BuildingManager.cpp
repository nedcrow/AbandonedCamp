// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingManager.h"
#include "TileSystem/BonFireComponent.h"
#include "TileSystem/StartPointTile.h"
#include "Kismet/GameplayStatics.h"

ABuildingManager* ABuildingManager::Instance_;

ABuildingManager* ABuildingManager::GetInstance()
{
	ABuildingManager* BM;
	if (Instance_) {
		BM = Instance_;
	}
	else {
		UWorld* world = GEngine->GameViewport->GetWorld();
		BM = Cast<ABuildingManager>(UGameplayStatics::GetActorOfClass(world, ABuildingManager::StaticClass()));
		if (!BM) {
			BM = world->SpawnActor<ABuildingManager>();
			#if WITH_EDITOR
				BM->SetFolderPath(TEXT("/Managers"));
			#endif // WITH_EDITOR
		}	
	}
	return BM;
}

ABuildingManager::ABuildingManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABuildingManager::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle timeHandle;
	GetWorldTimerManager().SetTimer(timeHandle, this, &ABuildingManager::UpdateManager, 0.5f, false);
}

void ABuildingManager::UpdateManager()
{
	// Update BuildingArr
	BuildingArr.Empty();
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Building"), BuildingArr);

	// UpdateFireBuildingArr
	MaxFireBuildingCount = FireBuildingArr.Num();
	FireBuildingArr.Empty();
	for (auto building : BuildingArr) {
		UBonFireComponent* tempFireComp = Cast<UBonFireComponent>(building->GetComponentByClass(UBonFireComponent::StaticClass()));
		if (tempFireComp) {
			FireBuildingArr.Add(building);
		}
	}
	if (MaxFireBuildingCount < FireBuildingArr.Num()) {
		MaxFireBuildingCount = FireBuildingArr.Num();
	}

	// Update StrangerStartPointArr
	StrangerStartPointArr.Empty();
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AStartPointTile::StaticClass(), TEXT("Stranger"), StrangerStartPointArr);
}

