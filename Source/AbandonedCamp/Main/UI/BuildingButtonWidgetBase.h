// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingButtonWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API UBuildingButtonWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UBorder* BuildingBorder;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UButton* BuildingButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UTextBlock* BuildingName;

	void InitButton(FName Name, UMaterialInstance* MI);
};
