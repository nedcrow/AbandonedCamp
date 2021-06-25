// Fill out your copyright notice in the Description page of Project Settings.


#include "BonFireComponent.h"
#include "Engine/World.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Landscape.h"

// Sets default values for this component's properties
UBonFireComponent::UBonFireComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBonFireComponent::PostEditComponentMove(bool bFinished)
{
	Super::PostEditComponentMove(bFinished);
	/*DestroyEffects();
	SpawnEffects();*/
}

void UBonFireComponent::BeginPlay()
{
	Super::BeginPlay();
	DestroyInteractionEffects();
	SpawnInteractionEffects();
}

void UBonFireComponent::SpawnInteractionEffects() {
	DeformateToLandscape();
}

void UBonFireComponent::DestroyInteractionEffects()
{
	ALandscape* land = Cast<ALandscape>(UGameplayStatics::GetActorOfClass(GetWorld(), ALandscape::StaticClass()));
	if (land) {
		// RenderTarget 위에 마스킹
		UCanvas* canvas;
		FVector origin;
		FVector boxExtent;
		land->GetActorBounds(false, origin, boxExtent);

		FVector2D screenSize = FVector2D(RenderTarget->SizeX, RenderTarget->SizeY);
		FDrawToRenderTargetContext targetContext;

		UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), RenderTarget, canvas, screenSize, targetContext);
		canvas->K2_DrawMaterial(EraseMaterial, LastTransform.Position, LastTransform.Size, FVector2D(0, 0));
		UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), targetContext);
	}
}

void UBonFireComponent::DeformateToLandscape()
{
	// LandScape 준비
	TArray<AActor*> snowLandArr;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("SnowLandscape"), snowLandArr);
	ALandscape* snowLand = snowLandArr.Num() > 0 ? Cast<ALandscape>(snowLandArr[0]) : nullptr;
	
	if (snowLand && RenderTarget) {
		// Tracing으로 위치 찾기
		FHitResult outHit;
		FVector traceStart = GetOwner()->GetActorLocation();
		FVector traceEnd = traceStart + (GetUpVector() * -9999.f);
		FCollisionQueryParams collisionParam;
		collisionParam.AddIgnoredActor(GetOwner());
		GetWorld()->LineTraceSingleByChannel(outHit, traceStart, traceEnd, ECollisionChannel::ECC_Visibility, collisionParam);

		// RenderTarget 위에 마스킹
		if (outHit.GetActor()) {
			UCanvas* canvas;
			FVector origin;
			FVector boxExtent;
			snowLand->GetActorBounds(false, origin, boxExtent);	
			
			FVector2D HitScreenPosition = FVector2D(
				abs(snowLand->GetActorLocation().X - outHit.Location.X) / (boxExtent.X * 2),
				abs(snowLand->GetActorLocation().Y - outHit.Location.Y) / (boxExtent.Y * 2)
			);
			FVector2D screenSize = FVector2D(RenderTarget->SizeX, RenderTarget->SizeY);
			FDrawToRenderTargetContext targetContext;
			
			UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), RenderTarget, canvas, screenSize, targetContext);
			FCanvasMaterialTransform CanvasMaterialTransform = GetCanvasMaterialTransform(HitScreenPosition, screenSize, FireLightRadius * 0.01f);
			LastTransform = CanvasMaterialTransform;
			canvas->K2_DrawMaterial(MaskingMaterial, CanvasMaterialTransform.Position, CanvasMaterialTransform.Size, FVector2D(0, 0));

			UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), targetContext);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Not found landscape had tag : SnowLandscape in BonFireComponent"));
	}
}

FCanvasMaterialTransform UBonFireComponent::GetCanvasMaterialTransform(FVector2D Position, FVector2D Size, float Scale)
{
	FCanvasMaterialTransform result;

	FVector2D halfPos = Position - 0.5f;
	FVector2D halfSize = Size * 0.5f;
	result.Position = (FVector2D(Size.X, Size.Y) * FVector2D(halfPos.X, halfPos.Y)) + (halfSize * (1 - Scale));
	result.Size = Size * Scale;

	return result;
}
