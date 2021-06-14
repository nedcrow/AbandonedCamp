// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingButtonWidgetBase.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Engine/StreamableManager.h"

void UBuildingButtonWidgetBase::NativeConstruct() {
	Super::NativeConstruct();
	BuildingBorder = Cast<UBorder>(GetWidgetFromName(TEXT("BuildingBorder")));

	BuildingButton = Cast<UButton>(GetWidgetFromName(TEXT("BuildingButton")));

	BuildingName = Cast<UTextBlock>(GetWidgetFromName(TEXT("BuildingName")));
}

void UBuildingButtonWidgetBase::InitButton(FName Name, UMaterialInstance* MI) {
	BuildingBorder->SetBrushFromMaterial(MI);
}