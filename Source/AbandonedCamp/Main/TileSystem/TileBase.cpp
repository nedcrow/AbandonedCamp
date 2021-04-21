// Fill out your copyright notice in the Description page of Project Settings.


#include "TileBase.h"
#include "../MainGS.h"
#include "TileSnapComponent.h"
#include "TileManager.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATileBase::ATileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ATileBase::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	SnapTileManager(bFinished);

}

void ATileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATileBase::SnapTileManager(bool bFinished) {

	ATileManager* tileManager = Cast<ATileManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ATileManager::StaticClass()));
	if (tileManager && bFinished) {
		TArray<FTransform> tileTransformsArray;
		TArray<float> tileDistArray;
		FVector myLocation = GetActorLocation();
		for (int i = 0; i < tileManager->DefaultTileISM->GetInstanceCount(); i++) {
			FTransform tempTransform;
			tileManager->DefaultTileISM->GetInstanceTransform(i, tempTransform, true);
			tileDistArray.Add(FVector().Dist(tempTransform.GetLocation(), myLocation));
			tileTransformsArray.Add(tempTransform);
		}
		tileDistArray.Sort([](float a, float b) {return a < b; });

		int targetIndex;
		for (int i = 0; i < tileManager->DefaultTileISM->GetInstanceCount(); i++) {
			FTransform tempTransform;
			tileManager->DefaultTileISM->GetInstanceTransform(i, tempTransform, true);
			if (FVector().Dist(tempTransform.GetLocation(), myLocation) == tileDistArray[0]) targetIndex = i;
		}
		RootComponent->SetWorldLocation(tileTransformsArray[targetIndex].GetLocation());	}
}
