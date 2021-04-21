// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileBase.h"
#include "StartPointTile.generated.h"

UCLASS()
class ABANDONEDCAMP_API AStartPointTile : public ATileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStartPointTile();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UBoxComponent* Box;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UStaticMeshComponent* BodyMesh;


protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
