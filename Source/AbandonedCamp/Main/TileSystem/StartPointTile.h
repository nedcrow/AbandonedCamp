// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileBase.h"
#include "StartPointTile.generated.h"

UCLASS()
class ABANDONEDCAMP_API AStartPointTile : public ATileBase
{
	GENERATED_BODY()
	
public:	
	AStartPointTile();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	bool bUseablePoint = true;

	UFUNCTION()
	void CallDelFunc_BuildingEvent();
};
