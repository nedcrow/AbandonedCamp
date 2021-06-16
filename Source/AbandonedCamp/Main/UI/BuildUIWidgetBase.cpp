// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildUIWidgetBase.h"
#include "BuildingButtonWidgetBase.h"
#include "../MainGS.h"
#include "../TileSystem/BuildingTile.h"
#include "../TileSystem/TileManager.h"

#include "Components/BoxComponent.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"

void UBuildUIWidgetBase::NativeConstruct() {
	Super::NativeConstruct();

	BuildingScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("BuildingScrollBox")));
	BuildingTabButton = Cast<UButton>(GetWidgetFromName(TEXT("BuildingTabButton")));
	BuildingTabButton->OnClicked.AddDynamic(this, &UBuildUIWidgetBase::OnClickBuildingTabButton);
	InitBuildingWidgets();
}

void UBuildUIWidgetBase::InitBuildingWidgets() {
	
	FStreamableManager loader;

	TArray<FBuildingDataStruct*> buildingDataArray;
	for (int TableIndex = 0; TableIndex < BuildingScrollBox->GetChildrenCount(); ++TableIndex)
	{
		if (!BuildingDataTable) {
			UE_LOG(LogTemp, Warning, TEXT("Error: Null BuildingDataTable in UBuildUIWidgetBase"));
			break;
		}
		FBuildingDataStruct* ItemData = BuildingDataTable->FindRow<FBuildingDataStruct>(FName(*(FString::FromInt(TableIndex))), TEXT(""));
		if(ItemData) buildingDataArray.Add(ItemData);
	}

	for (int i = 0; i < buildingDataArray.Num(); ++i) {
		UMaterialInstance* MI = loader.LoadSynchronous<UMaterialInstance>(buildingDataArray[i]->ItemImage);
		UBuildingButtonWidgetBase* buttonWidget = Cast<UBuildingButtonWidgetBase>(BuildingScrollBox->GetChildAt(i));
		buttonWidget->InitButton(FName(buildingDataArray[i]->ItemName), MI, buildingDataArray[i]->ItemActor);
	}	
}

void UBuildUIWidgetBase::OnClickBuildingTabButton()
{
	if (bIsActive == true) {
		bIsActive = false;
		UWidgetBlueprintGeneratedClass* WidgetAnim = Cast<UWidgetBlueprintGeneratedClass>(GetClass());
		for (UWidgetAnimation* Anim : WidgetAnim->Animations) {
			if (Anim->GetName() == TEXT("SlideIn_INST")) {
				PlayAnimation(Anim);
				CallOffBuildableTiles();
				break;
			}
		}
	}
	else {
		bIsActive = true;
		UWidgetBlueprintGeneratedClass* WidgetAnim = Cast<UWidgetBlueprintGeneratedClass>(GetClass());
		for (UWidgetAnimation* Anim : WidgetAnim->Animations) {
			if (Anim->GetName() == TEXT("SlideOut_INST")) {
				PlayAnimation(Anim);
				break;
			}
		}
	}
}

void UBuildUIWidgetBase::CallOffBuildableTiles()
{
	AMainGS* GS = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS) {
		ATileManager* TM = GS->GetTileManager();
		if (TM) TM->OffBuildableTile();
		if (GS->CurrentSelectedBuilding) GS->CurrentSelectedBuilding->Destroy();
	}
}
