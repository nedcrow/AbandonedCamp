// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPC.h"
#include "MainGS.h"
#include "TileSystem/TileSnapComponent.h"
#include "UI/MainUIWidgetBase.h"
#include "Kismet/GameplayStatics.h"


AMainPC::AMainPC() 
{
	bShowMouseCursor = true;
}

void AMainPC::BeginPlay() {
	Super::BeginPlay();
	if (MainUIWidgetClass && IsLocalPlayerController()) {
		MainUIWidgetObject = CreateWidget<UMainUIWidgetBase>(this, MainUIWidgetClass);
		MainUIWidgetObject->AddToViewport();

		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
	}
}

void AMainPC::SetCurrentSelectedBuildingLocation(FVector Location)
{
	AMainGS* GS = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS) {
		if (GS->CurrentSelectedBuilding) {

			GS->CurrentSelectedBuilding->SetActorLocation(Location);

			UTileSnapComponent* snapComp = Cast<UTileSnapComponent>(GS->CurrentSelectedBuilding->GetComponentByClass(UTileSnapComponent::StaticClass()));
			if (snapComp) {
				snapComp->SnapToTileManager(true);
			}
		}
	}
}