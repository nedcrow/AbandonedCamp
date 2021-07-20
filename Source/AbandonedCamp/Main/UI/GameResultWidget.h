// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API UGameResultWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
		class UTextBlock* TitleTextBlock;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
		class UTextBlock* CampTextBlock;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
		class UTextBlock* SurvivingTextBlock;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
		class UButton* TryAgainButton;

	virtual void NativeConstruct() override;

	void UpdateGameResult();

	UFUNCTION()
	void TryAgain();
};
