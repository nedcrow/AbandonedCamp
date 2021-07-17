// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CamperListWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API UCamperListWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UScrollBox* CamperScrollBox;

	virtual void NativeConstruct() override;

	void UpdateList();
};
