// Fill out your copyright notice in the Description page of Project Settings.


#include "TileBase.h"
#include "TileSnapComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATileBase::ATileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileSnapComponent = CreateDefaultSubobject<UTileSnapComponent>(TEXT("TileSnapComponent"));
}

void ATileBase::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);
}

void ATileBase::BeginPlay()
{
	Super::BeginPlay();	
}
