// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../TileSystem/BuildingTile.h"
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

	//Properties
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UBorder* BuildingBorder;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UButton* BuildingButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UTextBlock* BuildingName;

	//Data
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	TSubclassOf<class ABuildingTile> BuildingClass;

	//Init
	void InitButton(FName Name, UMaterialInstance* MI, UClass* Class);

	//Interaction
	UFUNCTION()
	void OnClickBuildingButton();
	void CallOnBuildableTiles();
	void CallOffBuildableTiles();
};
