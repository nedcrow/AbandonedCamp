// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIWidgetBase.h"
#include "CamperListWidgetBase.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMainUIWidgetBase::NativeConstruct()
{
	DayStateBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("DayStateBar")));
	DayTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("DayTextBlock")));
	CamperListWidget = Cast<UCamperListWidgetBase>(GetWidgetFromName(TEXT("CamperListWidget")));
}

void UMainUIWidgetBase::SetDayStateBar(float Percent)
{
	DayStateBar->SetPercent(Percent);
	if (Percent >= 1) {
		// Text
		int newDay = FCString::Atoi(*DayTextBlock->GetText().ToString()) + 1;
		DayTextBlock->SetText(FText::FromString(FString::FromInt(newDay)));

		// Text animation
		UWidgetBlueprintGeneratedClass* widgetAnim = Cast<UWidgetBlueprintGeneratedClass>(GetClass());
		for (UWidgetAnimation* anim : widgetAnim->Animations) {
			if (anim->GetName() == TEXT("NewDay_INST")) {
				PlayAnimation(anim);
				break;
			}
		}
	}
}
