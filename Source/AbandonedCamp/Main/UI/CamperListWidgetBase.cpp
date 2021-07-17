// Fill out your copyright notice in the Description page of Project Settings.


#include "CamperListWidgetBase.h"
#include "CamperWidgetBase.h"
#include "../SpawnManager.h"
#include "../Characters/CamperCharacter.h"

#include "Components/ScrollBox.h"

void UCamperListWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	CamperScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("CamperScrollBox")));
}

void UCamperListWidgetBase::UpdateList()
{
	ASpawnManager * SM = ASpawnManager::GetInstance();
	for (int i = 0; i < CamperScrollBox->GetChildrenCount(); i++) {		
		UCamperWidgetBase* camperWidget = Cast<UCamperWidgetBase>(CamperScrollBox->GetChildAt(i));
		
		if (i >= SM->SpawnedCamperActors.Num()) {
			camperWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		else {
			camperWidget->SetVisibility(ESlateVisibility::Visible);

			ACamperCharacter* camper = Cast<ACamperCharacter>(SM->SpawnedCamperActors[i]);

			// Thumnail
			UMaterialInstance* MI = camper->Thumbnail;
			if (MI)	camperWidget->SetCamperImage(MI);

			//Name
			FName camperName = camper->CamperName != NAME_None ? camper->CamperName : FName("");
			camperWidget->SetCamperName(camperName);

			// HPBar
			camperWidget->UpdateHPBar(camper->CurrentHP / camper->MaxHP);
		}
	}
}
