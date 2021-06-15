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

void UBuildingButtonWidgetBase::InitButton(FName Name, UMaterialInstance* MI) {
	BuildingBorder->SetBrushFromMaterial(MI);
}

void UBuildingButtonWidgetBase::OnClickBuildingButton()
{
	CallOnBuildableTiles();
}

void UBuildingButtonWidgetBase::CallOnBuildableTiles()
{
	AMainGS* GS = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS) {
		ATileManager* TM =GS->GetTileManager();
		if (TM) {
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