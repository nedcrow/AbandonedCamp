// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGM.h"
#include "MainPC.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MainUIWidgetBase.h"

AMainGM::AMainGM() {
	PrimaryActorTick.bCanEverTick = true;
	CurrentTime = 0.f;
}

void AMainGM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AMainPC* PC = Cast<AMainPC>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	if (PC && PC->MainUIWidgetObject) {
		if (CurrentTime < 1) {
			CurrentTime += GetWorld()->GetDeltaSeconds() / SecondPerDay;
		}
		else {
			CurrentTime = 0.f;
		}
		PC->MainUIWidgetObject->SetDayStateBar(CurrentTime);
	}
}
