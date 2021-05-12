// Fill out your copyright notice in the Description page of Project Settings.


#include "OnOffButtonWidgetBase.h"
#include "Components/CheckBox.h"

void UOnOffButtonWidgetBase::NativeConstruct() {
	Super::NativeConstruct();

	OnOffToggle = Cast<UCheckBox>(GetWidgetFromName(TEXT("OnOffToggle")));
}