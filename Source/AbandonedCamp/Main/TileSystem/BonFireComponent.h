// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BonFireComponent.generated.h"

USTRUCT(BlueprintType)
struct FCanvasMaterialTransform {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D Position;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D Size;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ABANDONEDCAMP_API UBonFireComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBonFireComponent();

public:
#if WITH_EDITOR
	virtual void PostEditComponentMove(bool bFinished) override;
#endif
	virtual void BeginPlay() override;

public:	

	// FireLights
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	class UTextureRenderTarget2D* RenderTarget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	int FireLightRadius = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	UMaterialInstance* MaskingMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	UMaterialInstance* EraseMaterial;

	FCanvasMaterialTransform LastTransform;

	UFUNCTION()
	void DestroyDeformated();

	UFUNCTION()
	void ClearRenderTarget();

	UFUNCTION()
	void DeformateToLandscape();

private:
	UFUNCTION()
	FCanvasMaterialTransform GetCanvasMaterialTransform(FVector2D Position, FVector2D Size, float Scale);
	
};
