// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "BuildUIWidgetBase.generated.h"

USTRUCT(BlueprintType)
struct ABANDONEDCAMP_API FBuildingDataStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ItemIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TAssetPtr<class UMaterialInstance> ItemImage;

	/*UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TAssetPtr<class USkeletalMesh> GunMesh;*/
};

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API UBuildUIWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	// Properties
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UScrollBox* BuildingScrollBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UButton* BuildingTabButton;

	//UPROPERTY(Meta = (BindWidgetAnim), Meta =(AllowPrivateAccess = true))
	//class UWidgetAnimation* SlideIn;

	//UPROPERTY(Meta = (BindWidgetAnim), Meta = (AllowPrivateAccess = true))
	//class UWidgetAnimation* SlideOut; // bluprint compile error


	// Data
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	class UDataTable* BuildingDataTable;

	// Init
	void InitBuildingWidgets();

	// BuildingTab
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	bool bIsActive = true;

	UFUNCTION()
	void OnClickBuildingTabButton();
	void CallOffBuildableTiles();
};
