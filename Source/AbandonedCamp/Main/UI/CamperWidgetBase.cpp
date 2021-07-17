// Fill out your copyright notice in the Description page of Project Settings.


#include "CamperWidgetBase.h"
#include "HPBarWidgetBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCamperWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	CamperImage = Cast<UImage>(GetWidgetFromName(TEXT("CamperImage")));
	CamperHPWidget = Cast<UHPBarWidgetBase>(GetWidgetFromName(TEXT("CamperHPWidget")));
	NameTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameTextBlock")));
}

void UCamperWidgetBase::SetCamperImage(UMaterialInstance* MI) {
	CamperImage->SetBrushFromMaterial(MI);
}

void UCamperWidgetBase::UpdateHPBar(float Percentage) {
	CamperHPWidget->SetHPBar(Percentage == 1 ? 0.99999f : Percentage);
}

void UCamperWidgetBase::SetCamperName(FName Name) {
	NameTextBlock->SetText(FText::FromName(Name));
}