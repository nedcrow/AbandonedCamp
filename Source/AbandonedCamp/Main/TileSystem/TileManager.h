// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileManager.generated.h"

UCLASS()
class ABANDONEDCAMP_API ATileManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileManager();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBoxComponent* Box;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UInstancedStaticMeshComponent* DefaultTileISM;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UInstancedStaticMeshComponent* BuildableISM;

protected:
	virtual void PostRegisterAllComponents() override;
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile")
	FVector CurrentTileLocation;

public:	
	UFUNCTION()
	FVector GetCurrentTileLocation();

#pragma region About TileMap
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TileMap")
	uint8 bIsMistTile : 1;

	/* X축 타일 갯수 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TileMap")
	int SizeX = 3;

	/* Y축 타일 갯수 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TileMap")
	int SizeY = 3;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TileMap")
	int TileScale = 1;

	void SpawnInstancedTilemap(int CountX, int CountY);

#pragma endregion

	
#pragma region About Tile Interaction
	// Properties for Tile UI
	/**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TileUI")
	UStaticMesh* SelectedMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TileUI")
	UMaterialInterface* Able_Build_MI;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TileUI")
	UMaterialInterface* Unable_Build_MI;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TileUI")
	UMaterialInterface* Border_MI;

	class UDecalComponent* CursorToWorld;

	// Hovered tile
	UFUNCTION()
	void CallDelFunc_TileHoveredEvent(bool isHovered);


	// Buildable tile
	void OnBuildableTile();
	void OffBuildableTile();

#pragma endregion	
};
