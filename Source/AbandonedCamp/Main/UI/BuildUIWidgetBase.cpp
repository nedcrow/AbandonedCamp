// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildUIWidgetBase.h"
#include "BuildingButtonWidgetBase.h"
#include "Components/ScrollBox.h"
#include "Engine/StreamableManager.h"

void UBuildUIWidgetBase::NativeConstruct() {
	Super::NativeConstruct();

	BuildingScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("BuildingScrollBox")));
	InitBuildingWidgets();
}

void UBuildUIWidgetBase::InitBuildingWidgets() {
	
	FStreamableManager loader;

	TArray<FBuildingDataStruct*> buildingDataArray;
	for (int TableIndex = 0; TableIndex < BuildingScrollBox->GetChildrenCount(); ++TableIndex)
	{
		if (!BuildingDataTable) break;
		FBuildingDataStruct* ItemData = BuildingDataTable->FindRow<FBuildingDataStruct>(FName(*(FString::FromInt(TableIndex))), TEXT(""));
		if(ItemData) buildingDataArray.Add(ItemData);
	}

	for (int i = 0; i < buildingDataArray.Num(); ++i) {
		UMaterialInstance* MI = loader.LoadSynchronous<UMaterialInstance>(buildingDataArray[i]->ItemImage);
		UBuildingButtonWidgetBase* buttonWidget = Cast<UBuildingButtonWidgetBase>(BuildingScrollBox->GetChildAt(i));
		buttonWidget->InitButton(FName(buildingDataArray[i]->ItemName), MI);	
	}	
}