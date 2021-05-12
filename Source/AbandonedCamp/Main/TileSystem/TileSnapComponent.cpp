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
* ���� ��� : �� ������Ʈ�� ���� Actor
* ���� ��ġ : TileManager ��Ʈ���� �� ���� ����� Actor
* ���� ���� : Edit ���¸� �����Ͽ� ���Ͱ� �������� �� (������ �ݵ�� PostEditComponentMove ���� ����)
*/
void UTileSnapComponent::SnapTileManager(bool bFinished)
{
	ATileManager* tileManager = Cast<ATileManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ATileManager::StaticClass()));
	if (tileManager && bFinished) {
		float socketPointZ = 0.f;
	
		// Ÿ�Ͽ� ������ �ִٸ� Snap ��ġ�� Z�� ����		
		if (tileManager->DefaultTileISM->GetStaticMesh()->Sockets.Num() > 0) {
			socketPointZ += tileManager->DefaultTileISM->GetStaticMesh()->Sockets[0]->RelativeLocation.Z;
		}

		// SnapableTransforms �迭�� ������ Transform���� ����
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

		// �Ÿ� �������� ����
		snapableTransforms.Sort([](FSnapableTransform a, FSnapableTransform b) {return a.Distance < b.Distance; });

		// ���� ����� ��ġ�� ���� ���͸� ��ġ (��ġ�� ���� ������ Box ũ�� �ݿ�)
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

