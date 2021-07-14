// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGM.generated.h"

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API AMainGM : public AGameModeBase
{
	GENERATED_BODY()
	public:
		AMainGM();

	public:
		virtual void Tick(float DeltaTime) override;

	// Level
	public:
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Level")
		int CamperCount = 1;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Level")
		int StrangerCount = 1;

	// Day
	public:
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Day")
		float CurrentTime;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Day")
		int SecondPerDay = 30;

};
