// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGS.h"
#include "MainPC.h"
#include "TileSystem/TileManager.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

AMainGS::AMainGS() {
	bReplicates = true;
}

void AMainGS::BeginPlay() {
	Super::BeginPlay();
}

void AMainGS::GetLifetimeReplicatedProps(TArray < FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMainGS, HoveredTileIndex);
}

void AMainGS::OnRep_ChangedTileIndex()
{
	AMainPC* PC = Cast<AMainPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	if (PC->IsLocalController()) {
		if (F_TileHoveredEvent.IsBound() == true) {
			if (HoveredTileIndex == -1) {
				F_TileHoveredEvent.Broadcast(false);
			}
			else {
				F_TileHoveredEvent.Broadcast(true);
			}
		}
	}

}

ATileManager* AMainGS::GetTileManager()
{
	return Cast<ATileManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ATileManager::StaticClass()));
}
