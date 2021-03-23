// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MainGS.generated.h"

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API AMainGS : public AGameStateBase
{
	GENERATED_BODY()
	public:
		AMainGS();

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tile")
		int hoveredTileIndex = -1;
};
