// Fill out your copyright notice in the Description page of Project Settings.


#include "TileManager.h"
#include "../MainGS.h"
#include "../MainPC.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ATileManager::ATileManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateAbstractDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	DefaultTileISM = CreateAbstractDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("DefaultTileISM"));
	DefaultTileISM->SetupAttachment(Box);
	DefaultTileISM->ComponentTags.Add("Tile");

	bReplicates = true;
}

void ATileManager::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();
#if WITH_EDITOR
	SetupDefaultTilemap(SizeX, SizeY);
#endif // WITH_EDITOR
}

void ATileManager::BeginPlay()
{
	Super::BeginPlay();
	AMainGS* gs = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	if(gs) gs->F_TileHoveredEvent.AddUFunction(this, FName("CallDelFunc_TileHoverdEvent"));
	SetupDefaultTilemap(SizeX, SizeY);
}

FVector ATileManager::GetCurrentTileLocation()
{
	return CurrentTileLocation;
}

// 타일 인스턴스들로 기본 타일맵 생성
void ATileManager::SetupDefaultTilemap(int CountX, int CountY)
{
	float oneUnit = 100 * TileScale;
	float startPointX = -(CountX * oneUnit * 0.5f) + (oneUnit * 0.5f); // 전체 타일 길이의 반 + 한 타일의 반
	float startPointY = -(CountY * oneUnit * 0.5f) + (oneUnit * 0.5f); // 전체 타일 길이의 반 + 한 타일의 반
	float startPointZ = GetActorLocation().Z;

	if (DefaultTileISM) {
		if(!DefaultTileISM->IsVisible()) DefaultTileISM->SetVisibility(true);
		if(DefaultTileISM->GetInstanceCount() > 0) DefaultTileISM->ClearInstances();

		for (int x = 0; x < CountX; x++) {
			for (int y = 0; y < CountY; y++) {
				DefaultTileISM->AddInstance(FTransform(
					FRotator().ZeroRotator,
					FVector(startPointX + x * oneUnit, startPointY + y * oneUnit, startPointZ),
					FVector(TileScale)
				));
			}
		}
		AMainGS* gs = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
		if (gs) gs->TotalTileCount = CountX * CountY;
	}
}

void ATileManager::CallDelFunc_TileHoverdEvent(bool isHovered)
{
	AMainGS* gs = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (gs) {
		// 필수 Material 확인
		if (Border_MI == nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("!!!- Null : border_MI -!!!"));
			return;
		}

		// Decal 위치 준비
		int tileIndex = gs->HoveredTileIndex;
		FTransform tileTransform;
		DefaultTileISM->GetInstanceTransform(tileIndex, tileTransform, true);
		FVector tileLocation = FVector(tileTransform.GetLocation().X, tileTransform.GetLocation().Y, tileTransform.GetLocation().Z + 0);

		// selectedTileDecal 없으면 생성
		if (CursorToWorld == nullptr) {
			CursorToWorld = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), Border_MI, FVector(50.0f, 50.0f, 100.0f), tileLocation, FRotator().ZeroRotator, 0.0f);
			//CursorToWorld->ComponentTags.Add("Tile");
		}

		// Decal 배치
		if (isHovered == true) {
			CursorToWorld->SetRelativeLocation(tileLocation);
			CursorToWorld->Activate(true);
			//UE_LOG(LogTemp, Warning, TEXT("TileManager _ Hovered!!!!"));
		}
		else {
			CursorToWorld->Activate(false);
			CursorToWorld->SetRelativeLocation(FVector(0.0f, 0.0f, -99999.0f));
			//UE_LOG(LogTemp, Warning, TEXT("TileManager _ Out!!!!"));
		}
	}
}


