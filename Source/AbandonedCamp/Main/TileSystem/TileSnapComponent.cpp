// Fill out your copyright notice in the Description page of Project Settings.


#include "TileSnapComponent.h"
#include "../MainGS.h"
#include "TileManager.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"

UTileSnapComponent::UTileSnapComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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
		float socketPointZ = 0.f;
	
		// 타일에 소켓이 있다면 Snap 위치의 Z값 조정		
		if (tileManager->DefaultTileISM->GetStaticMesh()->Sockets.Num() > 0) {
			socketPointZ += tileManager->DefaultTileISM->GetStaticMesh()->Sockets[0]->RelativeLocation.Z;
		}

		// SnapableTransforms 배열에 보정된 Transform들을 담음
		FVector myLocation = GetOwner()->GetActorLocation();
		TArray<FSnapableTransform> snapableTransforms = TArray<FSnapableTransform>();
		for (int i = 0; i < tileManager->DefaultTileISM->GetInstanceCount(); i++) {
			FSnapableTransform tempSnapableTransform;
			FTransform tempTransform;
			tileManager->DefaultTileISM->GetInstanceTransform(i, tempTransform, true);
			tempSnapableTransform.Transform = FTransform(
				tempTransform.GetRotation(),
				FVector(tempTransform.GetLocation().X, tempTransform.GetLocation().Y, tempTransform.GetLocation().Z + socketPointZ),
				tempTransform.GetScale3D()
			);
			tempSnapableTransform.Distance = FVector().Dist(tempTransform.GetLocation(), myLocation);
			snapableTransforms.Add(tempSnapableTransform);
		}

		// 거리 오름차순 정렬
		snapableTransforms.Sort([](FSnapableTransform a, FSnapableTransform b) {return a.Distance < b.Distance; });

		// 가장 가까운 위치에 상위 액터를 배치 (위치에 상위 액터의 Box 크기 반영)
		if (snapableTransforms.Num() > 0) {
			UBoxComponent* tempBox = Cast<UBoxComponent>(GetOwner()->GetComponentByClass(UBoxComponent::StaticClass()));
			float addZ = tempBox ? tempBox->GetScaledBoxExtent().Z : 0.f;

			FVector tempLocation = FVector(
				snapableTransforms[0].Transform.GetLocation().X,
				snapableTransforms[0].Transform.GetLocation().Y,
				snapableTransforms[0].Transform.GetLocation().Z + addZ
			);
			GetOwner()->SetActorRelativeLocation(tempLocation);
		}
	}
}

void UTileSnapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

