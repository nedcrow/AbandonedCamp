// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPC.h"
#include "UI/MainUIWidgetBase.h"


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