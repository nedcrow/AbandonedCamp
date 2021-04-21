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
* ���� ��� : �� ������Ʈ�� ���� Actor
* ���� ��ġ : TileManager ��Ʈ���� �� ���� ����� Actor
* ���� ���� : Edit ���¸� �����Ͽ� ���Ͱ� �������� �� (������ �ݵ�� PostEditComponentMove ���� ����)
*/
void UTileSnapComponent::SnapTileManager(bool bFinished)
{
	ATileManager* tileManager = Cast<ATileManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ATileManager::StaticClass()));
	if (tileManager && bFinished) {
		TArray<FSnapableTransform> snapableTransforms = TArray<FSnapableTransform>();
		FVector myLocation = GetOwner()->GetActorLocation();

		// snapableTransforms ����
		for (int i = 0; i < tileManager->DefaultTileISM->GetInstanceCount(); i++) {
			FSnapableTransform tempSnapableTransform;
			FTransform tempTransform;
			tileManager->DefaultTileISM->GetInstanceTransform(i, tempTransform, true);
			tempSnapableTransform.Transform = tempTransform;
			tempSnapableTransform.Distance = FVector().Dist(tempTransform.GetLocation(), myLocation);
			snapableTransforms.Add(tempSnapableTransform);
		}
		snapableTransforms.Sort([](FSnapableTransform a, FSnapableTransform b) {return a.Distance < b.Distance; });

		// ���� ����� ��ġ�� ���� ���� ��ġ
		if (snapableTransforms.Num() > 0)GetOwner()->SetActorRelativeLocation(snapableTransforms[0].Transform.GetLocation());
	}
}

void UTileSnapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

