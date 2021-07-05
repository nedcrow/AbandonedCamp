// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingManager.generated.h"

UCLASS()
class ABANDONEDCAMP_API ABuildingManager : public AActor
{
	GENERATED_BODY()

protected:
	static ABuildingManager* Instance_;
public:
	static ABuildingManager* GetInstance();
public:
	ABuildingManager();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<AActor*> BuildingArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<AActor*> FireBuildingArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<AActor*> StrangerStartPointArr;

	void UpdateManager();

};
