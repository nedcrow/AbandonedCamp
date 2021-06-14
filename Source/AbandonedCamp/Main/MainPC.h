// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPC.generated.h"

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API AMainPC : public APlayerController
{
	GENERATED_BODY()
public:
	AMainPC();

private:
	virtual void BeginPlay() override;


public: // UI
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	TSubclassOf<class UMainUIWidgetBase> MainUIWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	class UMainUIWidgetBase* MainUIWidgetObject;
};
