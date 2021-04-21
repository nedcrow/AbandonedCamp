// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileBase.generated.h"

UCLASS()
class ABANDONEDCAMP_API ATileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileBase();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTileSnapComponent* TileSnapComponent;

protected:
	virtual void PostEditMove(bool bFinished) override;
	virtual void BeginPlay() override;

public:	
	
};
