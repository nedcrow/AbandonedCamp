// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TileSnapComponent.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FSnapableTransform
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Distance;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ABANDONEDCAMP_API UTileSnapComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTileSnapComponent();

protected:
	
	virtual void PostEditComponentMove(bool bFinished) override;

public:
	/*
	* 스냅 대상 : 이 컴포넌트를 가진 Actor
	* 스냅 위치 : TileManager 매트릭스 중 가장 가까운 Actor
	*/
	void SnapToTileManager(bool bFinished);
};
