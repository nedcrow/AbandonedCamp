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
	class USceneComponent* Scene;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UStaticMeshComponent* CollisionPlane;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBoxComponent* BoxCollision;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UInstancedStaticMeshComponent* DefaultTileISM;

protected:
	// Called when the game starts or when spawned
	virtual void PreRegisterAllComponents() override;
	virtual void PostRegisterAllComponents() override;
	virtual void PostActorCreated() override;
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ActiveTileManager();

	// TileMap
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="TileMap")
	int sizeX = 3;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TileMap")
	int sizeY = 3;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TileMap")
	int tileScale = 1;

	// Tile
	/* for decal materials */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tile")
	UStaticMesh* tileMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tile")
	UMaterialInterface* able_Build_MI;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tile")
	UMaterialInterface* unable_Build_MI;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tile")
	UMaterialInterface* border_MI;

	void SetupDefaultTiles(int CountX, int CountY);
};
