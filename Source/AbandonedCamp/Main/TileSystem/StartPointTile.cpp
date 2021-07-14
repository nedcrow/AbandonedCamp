// Fill out your copyright notice in the Description page of Project Settings.


#include "StartPointTile.h"
#include "TileManager.h"
#include "../MainGS.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AStartPointTile::AStartPointTile()
{
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(FName("StartPoint"));
}

void AStartPointTile::BeginPlay()
{
	Super::BeginPlay();
	BodyMesh->SetVisibility(false);

	// Bind Event
	AMainGS* GS = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS) GS->F_TileHoveredEvent.AddUFunction(this, FName("CallDelFunc_BuildingEvent"));
}

void AStartPointTile::CallDelFunc_BuildingEvent()
{
	AMainGS* GS = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS) {
		ATileManager* TM = GS->GetTileManager();
		for (int i = 0; i < TM->BuildableLocations.Num(); i++) {
			if (TM->BuildableLocations[i].X == GetActorLocation().X && TM->BuildableLocations[i].Y == GetActorLocation().Y) {
				bUseablePoint = false;
			}
			else {
				bUseablePoint = true;
			}
		}
	}
}

