// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingManager.generated.h"

/* 
* 빌딩 목록 관리
* - GetInstance: 액터가 없으면 새로 생성하여 반환.
*/
UCLASS()
class ABANDONEDCAMP_API ABuildingManager : public AActor
{
	GENERATED_BODY()

private:
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

	int MaxFireBuildingCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<AActor*> StrangerStartPointArr;

	/* 빌딩 목록(BuildingArr, FireBuildingArr, StrangerStartPointArr) 갱신. */
	void UpdateManager();
};
