// Fill out your copyright notice in the Description page of Project Settings.


#include "GameResultWidget.h"
#include "../BuildingManager.h"
#include "../SpawnManager.h"
#include "../Characters/CamperCharacter.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameResultWidget::NativeConstruct()
{
	TitleTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TitleTextBlock")));
	CampTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("CampTextBlock")));
	SurvivingTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("SurvivingTextBlock")));
	TryAgainButton = Cast<UButton>(GetWidgetFromName(TEXT("TryAgainButton")));

	if (TryAgainButton)
	{
		TryAgainButton->OnClicked.AddDynamic(this, &UGameResultWidget::TryAgain);
	}
}

void UGameResultWidget::UpdateGameResult() {
	ABuildingManager* BM = ABuildingManager::GetInstance();
	ASpawnManager* SM = ASpawnManager::GetInstance();

	int camperCount = SM->SpawnedCamperActors.Num();
	int deadCount = 0;
	for (int i = 0; i < camperCount; i++) {
		ACamperCharacter* camper = Cast<ACamperCharacter>(SM->SpawnedCamperActors[i]);
		if(camper->CurrentState == ECharacterState::Dead) deadCount++;
	}

	if (camperCount == deadCount) {
		SetVisibility(ESlateVisibility::Visible);
		CampTextBlock->SetText(FText().FromString(FString("Camp : ") + FString::FromInt(BM->MaxFireBuildingCount)));
		SurvivingTextBlock->SetText(FText().FromString(FString("Surviving : ") + FString::FromInt(deadCount)));
	}
	else {
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGameResultWidget::TryAgain(){
	UE_LOG(LogTemp,Warning,TEXT("MainLevel"));
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainLevel"));
}