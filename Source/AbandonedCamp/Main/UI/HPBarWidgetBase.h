// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPBarWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API UHPBarWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
		class UProgressBar* HPBar;

public:
	virtual void NativeConstruct()override;

	void SetHPBar(float Percent);
	void SetHPBarColor(FLinearColor Color, FLinearColor BGColor = FLinearColor::Black);
};
