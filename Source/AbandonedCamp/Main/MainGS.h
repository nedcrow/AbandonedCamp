// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MainGS.generated.h"

UENUM(BlueprintType)
enum class ENightState : uint8
{
	Day		= 0		UMETA(DisplayName = "Day"),
	Night	= 1		UMETA(DisplayName = "Night"),
};

UENUM(BlueprintType)
enum class EUIState : uint8
{
	Normal	UMETA(DisplayName = "Normal"),
	Build	UMETA(DisplayName = "Build"),
	Menu	UMETA(DisplayName = "Menu"),
};

DECLARE_MULTICAST_DELEGATE_OneParam(FTileStateDelegate_OneParam, ENightState);
//DECLARE_MULTICAST_DELEGATE_OneParam(FUIStateDelegate_OneParam, EUIState);
DECLARE_MULTICAST_DELEGATE_OneParam(FTileHoveredDelegate_OneParam, bool);
DECLARE_MULTICAST_DELEGATE_TwoParams(FTouchDelegate_TwoParams, FName, FVector);

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API AMainGS : public AGameStateBase
{
	GENERATED_BODY() 

	public:
		AMainGS();

		virtual void PostInitializeComponents() override;
		virtual void BeginPlay() override;
		virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	public:
		// Status
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
		int TotalTileCount = 0;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
		int CurrentNight = 0;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
		EUIState CurrentUIState;

		// OnOffNightEvent
		UPROPERTY(ReplicatedUsing = OnRep_ChangedCurrentNight, VisibleAnywhere, BlueprintReadOnly, Category = "Status")
		ENightState NightState;

		UFUNCTION()
		void OnRep_ChangedCurrentNight();
		FTileStateDelegate_OneParam F_OnNightEvent;

		// Build
		AActor* CurrentSelectedBuilding;

		// HoveredTileEvent
		UPROPERTY(ReplicatedUsing = OnRep_ChangedTileIndex, VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
		int HoveredTileIndex = -1;

		UFUNCTION()
		void OnRep_ChangedTileIndex();
		FTileHoveredDelegate_OneParam F_TileHoveredEvent;

		// TouchedActorEvent
		UPROPERTY(ReplicatedUsing = OnRep_ChangedTileIndex, VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
		FName CurrentActorName;
		FVector CurrentActorLocation;

		UFUNCTION()
		void OnRep_ChangedCurrentActorName();
		FTouchDelegate_TwoParams F_TouchEvent;

		// ETC
		UFUNCTION()
		ATileManager* GetTileManager();
};
