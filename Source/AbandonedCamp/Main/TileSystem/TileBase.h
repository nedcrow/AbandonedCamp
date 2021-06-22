// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileBase.generated.h"

UCLASS()
class ABANDONEDCAMP_API ATileBase : public AActor
{
	GENERATED_BODY()
	
private:
	virtual void PostRegisterAllComponents() override;

public:	
	// Sets default values for this actor's properties
	ATileBase();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UBoxComponent* Box;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UStaticMeshComponent* BodyMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTileSnapComponent* TileSnapComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data_Size")
	FVector TileSize = FVector().OneVector;
};
