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
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(50.0f));

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);
}

void AStartPointTile::BeginPlay()
{
	Super::BeginPlay();
	BodyMesh->SetVisibility(false);
}

// Called every frame
void AStartPointTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

