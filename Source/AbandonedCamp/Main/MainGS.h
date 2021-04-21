// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MainGS.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FTileHoveredDelegate_OneParam, bool);

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API AMainGS : public AGameStateBase
{
	GENERATED_BODY()

	public:
		AMainGS();

		virtual void BeginPlay() override;
		virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
		
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
			int TotalTileCount = 0;

		UPROPERTY(ReplicatedUsing = OnRep_ChangedTileIndex, VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
			int HoveredTileIndex = -1;

		UFUNCTION()
			void OnRep_ChangedTileIndex();

		UFUNCTION()
			ATileManager* GetTileManager();

		FTileHoveredDelegate_OneParam F_TileHoveredEvent;

		
};
