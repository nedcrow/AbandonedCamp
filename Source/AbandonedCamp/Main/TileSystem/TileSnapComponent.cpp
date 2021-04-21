// Fill out your copyright notice in the Description page of Project Settings.


#include "TileSnapComponent.h"
#include "../MainGS.h"
#include "TileManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values for this component's properties
UTileSnapComponent::UTileSnapComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

}

void UTileSnapComponent::PostInitProperties()
{
	Super::PostInitProperties();
}

void UTileSnapComponent::PostLoad()
{
	Super::PostLoad();
}

void UTileSnapComponent::PreEditUndo()
{
	Super::PreEditUndo();
	UE_LOG(LogClass, Warning, TEXT("PostEditUndo - gs"));
	/*AMainGS* gs = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (gs) UE_LOG(LogTemp, Warning, TEXT("PostEditUndo - gs"));

	ATileManager* tileManager = gs ? gs->GetTileManager() : nullptr;
	if (tileManager) {
		GetOwner()->SetActorLocation(tileManager->GetCurrentTileLocation());
		UE_LOG(LogTemp, Warning, TEXT("PostEditUndo - tileManager"));
	}*/
}

void UTileSnapComponent::PostEditUndo()
{
	UE_LOG(LogClass, Warning, TEXT("PostEditUndo"));
}

void UTileSnapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

