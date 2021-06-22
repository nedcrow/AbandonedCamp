// Fill out your copyright notice in the Description page of Project Settings.


#include "TileBase.h"
#include "TileSnapComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATileBase::ATileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// RootComponent ����
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(50.0f));

	// BodyMesh ���� �� ��ġ�� Box �ٴ����� �̵�
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->AddRelativeLocation(FVector(0.f, 0.f, -Box->GetScaledBoxExtent().Z));

	TileSnapComponent = CreateDefaultSubobject<UTileSnapComponent>(TEXT("TileSnapComponent"));
}

void ATileBase::PostRegisterAllComponents() {
	Super::PostRegisterAllComponents();
	// Box ũ�� ���� �� BodyMesh ��ġ�� Box �ٴ����� �̵�
	BodyMesh->SetRelativeLocation(FVector(BodyMesh->GetRelativeLocation().X, BodyMesh->GetRelativeLocation().Y, -Box->GetScaledBoxExtent().Z));
}
