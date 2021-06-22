// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingButtonWidgetBase.h"
#include "../MainGS.h"
#include "../TileSystem/TileManager.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StreamableManager.h"

void UBuildingButtonWidgetBase::NativeConstruct() {
	Super::NativeConstruct();
	BuildingBorder = Cast<UBorder>(GetWidgetFromName(TEXT("BuildingBorder")));

	BuildingButton = Cast<UButton>(GetWidgetFromName(TEXT("BuildingButton")));
	BuildingButton->OnClicked.AddDynamic(this, &UBuildingButtonWidgetBase::OnClickBuildingButton);

	BuildingName = Cast<UTextBlock>(GetWidgetFromName(TEXT("BuildingName")));
}

void UBuildingButtonWidgetBase::InitButton(FName Name, UMaterialInstance* MI, UClass* Class) {

	BuildingBorder->SetBrushFromMaterial(MI);
	BuildingName->SetText(FText::FromName(Name));
	BuildingClass = Class;
}

void UBuildingButtonWidgetBase::OnClickBuildingButton()
{
	CallOnBuildableTiles();

	AMainGS* GS = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS) {
		// 선택된 빌딩이 있으면 삭제
		if (GS->CurrentSelectedBuilding) {
			GS->CurrentSelectedBuilding->Destroy();
		}

		// 새 빌딩 생성
		ABuildingTile* buildingActor = GetWorld()->SpawnActor<ABuildingTile>(BuildingClass, FVector(0.f, 0.f, -999.f), FRotator().ZeroRotator);
		if (buildingActor) {
			GS->CurrentSelectedBuilding = buildingActor;
			GS->CurrentUIState = EUIState::Build;
		}
	}
}

void UBuildingButtonWidgetBase::CallOnBuildableTiles()
{
	AMainGS* GS = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS) {
		ATileManager* TM = GS->GetTileManager();
		if (TM && TM->BuildableLocations.Num() == 0) {
			TM->OnBuildableTile();
		}
	}
}

void UBuildingButtonWidgetBase::CallOffBuildableTiles()
{
	AMainGS* GS = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS) {
		ATileManager* TM = GS->GetTileManager();
		if (TM) {
			TM->OffBuildableTile();
		}
	}
}