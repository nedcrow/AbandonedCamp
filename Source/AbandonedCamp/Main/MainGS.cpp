// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGS.h"
#include "MainPC.h"
#include "BuildingManager.h"
#include "SpawnManager.h"
#include "TileSystem/TileManager.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"


AMainGS::AMainGS() {
	bReplicates = true;
}

void AMainGS::PostInitializeComponents() {
	Super::PostInitializeComponents();
	ABuildingManager::GetInstance();
	ASpawnManager::GetInstance();
}

void AMainGS::BeginPlay() {
	Super::BeginPlay();
}

void AMainGS::GetLifetimeReplicatedProps(TArray < FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMainGS, HoveredTileIndex);
	DOREPLIFETIME(AMainGS, CurrentActorName);
}

void AMainGS::OnRep_ChangedCurrentNight()
{
	AMainPC* PC = Cast<AMainPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PC->IsLocalController()) {
		if (F_OnNightEvent.IsBound() == true) {
			if (NightState == ENightState::Day) {
				F_OnNightEvent.Broadcast(ENightState::Day);
			}
			else if(NightState == ENightState::Night){
				F_OnNightEvent.Broadcast(ENightState::Night);
			}
			
		}
	}
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

void AMainGS::OnRep_ChangedCurrentActorName()
{
	AMainPC* PC = Cast<AMainPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PC->IsLocalController()) {
		F_TouchEvent.Broadcast(CurrentActorName, CurrentActorLocation);
	}
}

void AMainGS::CallBuildingEvent()
{
	AMainPC* PC = Cast<AMainPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PC->IsLocalController()) {
		F_BuildingEvent.Broadcast();
	}
}

ATileManager* AMainGS::GetTileManager()
{
	return Cast<ATileManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ATileManager::StaticClass()));
}