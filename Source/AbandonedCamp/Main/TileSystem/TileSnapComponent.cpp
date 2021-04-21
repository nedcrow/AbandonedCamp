// Fill out your copyright notice in the Description page of Project Settings.


#include "TileSnapComponent.h"
#include "../MainGS.h"
#include "TileManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values for this component's properties
UTileSnapComponent::UTileSnapComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

}

void UTileSnapComponent::PostEditComponentMove(bool bFinished)
{
	Super::PostEditComponentMove(bFinished);
	SnapTileManager(bFinished);
}

/*
* 스냅 대상 : 이 컴포넌트를 가진 Actor
* 스냅 위치 : TileManager 매트릭스 중 가장 가까운 Actor
* 스냅 시점 : Edit 상태를 포함하여 액터가 움직였을 때 (때문에 반드시 PostEditComponentMove 통해 실행)
*/
void UTileSnapComponent::SnapTileManager(bool bFinished)
{
	ATileManager* tileManager = Cast<ATileManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ATileManager::StaticClass()));
	if (tileManager && bFinished) {
		TArray<FSnapableTransform> snapableTransforms = TArray<FSnapableTransform>();
		FVector myLocation = GetOwner()->GetActorLocation();

		// snapableTransforms 담음
		for (int i = 0; i < tileManager->DefaultTileISM->GetInstanceCount(); i++) {
			FSnapableTransform tempSnapableTransform;
			FTransform tempTransform;
			tileManager->DefaultTileISM->GetInstanceTransform(i, tempTransform, true);
			tempSnapableTransform.Transform = tempTransform;
			tempSnapableTransform.Distance = FVector().Dist(tempTransform.GetLocation(), myLocation);
			snapableTransforms.Add(tempSnapableTransform);
		}
		snapableTransforms.Sort([](FSnapableTransform a, FSnapableTransform b) {return a.Distance < b.Distance; });

		// 가장 가까운 위치에 상위 액터 배치
		if (snapableTransforms.Num() > 0)GetOwner()->SetActorRelativeLocation(snapableTransforms[0].Transform.GetLocation());
	}
}

void UTileSnapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

