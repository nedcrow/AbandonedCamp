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
}

