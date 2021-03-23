// Fill out your copyright notice in the Description page of Project Settings.


#include "TileManager.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
ATileManager::ATileManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateAbstractDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Scene->SetupAttachment(RootComponent);

	CollisionPlane = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("CollisionPlane"));
	CollisionPlane->SetupAttachment(Scene);

	BoxCollision = CreateAbstractDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(Scene);

	DefaultTileISM = CreateAbstractDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("DefaultTileISM"));
	DefaultTileISM->SetupAttachment(Scene);
	DefaultTileISM->ComponentTags.Add("ISM");
}

void ATileManager::PreRegisterAllComponents()
{
	Super::PreRegisterAllComponents();
}

void ATileManager::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();
	SetupDefaultTiles(sizeX, sizeY);
}

void ATileManager::PostActorCreated()
{
	Super::PostActorCreated();
}

// Called when the game starts or when spawned
void ATileManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATileManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATileManager::ActiveTileManager()
{
}


void ATileManager::SetupDefaultTiles(int CountX, int CountY)
{
	float oneUnit = 100 * tileScale;
	float startXPoint = -(CountX * oneUnit * 0.5f) + (oneUnit * 0.5f); // 전체 타일 길이의 반 + 한 타일의 반
	float startYPoint = -(CountY * oneUnit * 0.5f) + (oneUnit * 0.5f); // 전체 타일 길이의 반 + 한 타일의 반
	if (DefaultTileISM) {
		if(!DefaultTileISM->IsVisible()) DefaultTileISM->SetVisibility(true);
		if(DefaultTileISM->GetInstanceCount() > 0) DefaultTileISM->ClearInstances();

		for (int x = 0; x < CountX; x++) {
			for (int y = 0; y < CountY; y++) {
				DefaultTileISM->AddInstance(FTransform(
					FRotator().ZeroRotator,
					FVector(startXPoint + x * oneUnit, startYPoint + y * oneUnit, GetActorLocation().Z),
					FVector(tileScale)
				));
			}
		}
	}
}

