// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingManager.generated.h"

/* 
* ���� ��� ����
* - GetInstance: ���Ͱ� ������ ���� �����Ͽ� ��ȯ.
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

	/* ���� ���(BuildingArr, FireBuildingArr, StrangerStartPointArr) ����. */
	void UpdateManager();
};
