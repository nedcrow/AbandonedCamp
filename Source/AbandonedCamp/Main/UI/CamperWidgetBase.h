// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CamperWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API UCamperWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
		class UImage* CamperImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
		class UHPBarWidgetBase* CamperHPWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
		class UTextBlock* NameTextBlock;

	virtual void NativeConstruct() override;

	void SetCamperImage(UMaterialInstance* MI);

	void UpdateHPBar(float Percentage);

	void SetCamperName(FName Name);
};
