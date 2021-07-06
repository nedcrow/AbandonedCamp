// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUIWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API UMainUIWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UProgressBar* DayStateBar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UTextBlock* DayTextBlock;

public:
	virtual void NativeConstruct()override;

	void SetDayStateBar(float Percent);
};
