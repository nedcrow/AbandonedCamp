// Fill out your copyright notice in the Description page of Project Settings.


#include "HPBarWidgetBase.h"
#include "Components/ProgressBar.h"

void UHPBarWidgetBase::NativeConstruct()
{
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
}

void UHPBarWidgetBase::SetHPBar(float Percent)
{
	HPBar->SetPercent(Percent);

	if (Percent >= 1) {
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		SetVisibility(ESlateVisibility::Visible);
	}
}

void UHPBarWidgetBase::SetHPBarColor(FLinearColor Color, FLinearColor BGColor)
{
	FProgressBarStyle style = FProgressBarStyle();
	style.FillImage.TintColor = FSlateColor(Color);
	style.BackgroundImage.TintColor = FSlateColor(BGColor);
	HPBar->WidgetStyle = style;
}
