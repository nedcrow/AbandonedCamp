// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingManager.h"
#include "TileSystem/BonFireComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABuildingManager::ABuildingManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABuildingManager::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle timeHandle;
	GetWorldTimerManager().SetTimer(timeHandle, this, &ABuildingManager::UpdateManager, 0.5f, false);

}

// Called every frame
void ABuildingManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildingManager::UpdateManager()
{
	BuildingArr.Empty();
	FireBuildingArr.Empty();

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Building"), BuildingArr);

	for (auto building : BuildingArr) {
		UBonFireComponent* tempFireComp = Cast<UBonFireComponent>(building->GetComponentByClass(UBonFireComponent::StaticClass()));
		if (tempFireComp) {
			FireBuildingArr.Add(building);
		}
	}
}

