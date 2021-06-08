// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MistPlane.generated.h"

UCLASS()
class ABANDONEDCAMP_API AMistPlane : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMistPlane();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UStaticMeshComponent* Plane;

protected:
	virtual void PostInitializeComponents() override;

public:	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
		class UTextureRenderTarget2D* RenderTarget;

};
