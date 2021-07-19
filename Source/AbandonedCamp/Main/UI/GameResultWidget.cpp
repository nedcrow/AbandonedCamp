// Fill out your copyright notice in the Description page of Project Settings.


#include "GameResultWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UGameResultWidget::NativeConstruct()
{
	TitleTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TitleTextBlock")));
	CampTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("CampTextBlock")));
	SurvivingTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("SurvivingTextBlock")));
	TrayAgainButton = Cast<UButton>(GetWidgetFromName(TEXT("TrayAgainButton")));
}
