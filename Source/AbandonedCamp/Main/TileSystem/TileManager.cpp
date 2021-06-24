// Fill out your copyright notice in the Description page of Project Settings.


#include "TileManager.h"
#include "BonFireComponent.h"
#include "../MainGS.h"
#include "../MainPC.h"
#include "../BuildingManager.h"

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

	BuildableISM = CreateAbstractDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BuildableISM"));
	BuildableISM->SetupAttachment(Box);

	bReplicates = true;
}

void ATileManager::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();
#if WITH_EDITOR
	SpawnInstancedTilemap(SizeX, SizeY);
#endif // WITH_EDITOR
}

void ATileManager::BeginPlay()
{
	Super::BeginPlay();
	AMainGS* gs = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	if(gs) gs->F_TileHoveredEvent.AddUFunction(this, FName("CallDelFunc_TileHoveredEvent"));
	SpawnInstancedTilemap(SizeX, SizeY);
}

FVector ATileManager::GetCurrentTileLocation()
{
	return CurrentTileLocation;
}

// 타일 인스턴스들로 기본 타일맵 생성
void ATileManager::SpawnInstancedTilemap(int CountX, int CountY)
{
	float oneUnit = 100 * TileScale;
	float startPointX = -(CountX * oneUnit * 0.5f) + (oneUnit * 0.5f); // 전체 타일 길이의 반 + 한 타일의 반
	float startPointY = -(CountY * oneUnit * 0.5f) + (oneUnit * 0.5f); // 전체 타일 길이의 반 + 한 타일의 반
	float startPointZ = GetActorLocation().Z;

	// DefaultTile
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

void ATileManager::CallDelFunc_TileHoveredEvent(bool isHovered)
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
		FVector tileLocation = CurrentTileLocation = FVector(tileTransform.GetLocation().X, tileTransform.GetLocation().Y, tileTransform.GetLocation().Z + 0);

		// 커서 위치 확인 용 Decal 없으면 생성
		if (CursorTileDecal == nullptr) {
			CursorTileDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), Border_MI, FVector(50.0f, 50.0f, 100.0f), tileLocation, FRotator().ZeroRotator, 0.0f);
		}

		// 빌드 모드에 사용 할 Decal 없으면 생성
		if (BuildUnableTileDecal == nullptr) {
			BuildUnableTileDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), Unable_Build_MI, FVector(50.0f, 50.0f, 100.0f), tileLocation, FRotator().ZeroRotator, 0.0f);
		}

		// Decal 배치
		if (isHovered == true) {
			// CursorTileDecal : 노멀 모드에서 활성화
			if (gs->CurrentUIState == EUIState::Normal) {
				CursorTileDecal->SetRelativeLocation(tileLocation);
				CursorTileDecal->Activate(true);
			}
			else {
				CursorTileDecal->Activate(false);
				CursorTileDecal->SetRelativeLocation(FVector(0.0f, 0.0f, -99999.0f));
			}

			// BuildUnableTileDecal : 빌드모드면서 건설 불가능 지역에서 활성화
			bool canActive = gs->CurrentUIState == EUIState::Build;
			for (auto location : gs->GetTileManager()->BuildableLocations) {
				if (location.X == tileLocation.X && location.Y == tileLocation.Y) {
					canActive = false;
					break;
				}
			}
			if (canActive) {				
				BuildUnableTileDecal->SetRelativeLocation(tileLocation);
				BuildUnableTileDecal->Activate(true);
			}
			else {
				BuildUnableTileDecal->Activate(false);
				BuildUnableTileDecal->SetRelativeLocation(FVector(0.0f, 0.0f, -99999.0f));
			}
		}
		else {
			CursorTileDecal->Activate(false);
			CursorTileDecal->SetRelativeLocation(FVector(0.0f, 0.0f, -99999.0f));
			BuildUnableTileDecal->Activate(false);
			BuildUnableTileDecal->SetRelativeLocation(FVector(0.0f, 0.0f, -99999.0f));
		}
	}

	AMainPC* pc = Cast<AMainPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	pc->SetCurrentSelectedBuildingLocation(CurrentTileLocation);
}

void ATileManager::OnBuildableTile()
{
	if (Able_Build_MI == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("!!!- Null : border_MI -!!!"));
		return;
	}

	// 타일 위치 계산 (8각형)
	BuildableLocations.Empty();
	ABuildingManager* buildingM = Cast<ABuildingManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABuildingManager::StaticClass()));
	for (auto fire : buildingM->FireBuildingArr) {
		UBonFireComponent* fireComponent = Cast<UBonFireComponent>(fire->GetComponentByClass(UBonFireComponent::StaticClass()));
		int loopCountX = fireComponent->FireLightRadius;
		int loopCountY = fireComponent->FireLightRadius;
		float oneUnit = 100 * TileScale;

		UBoxComponent* fireBoxComp = Cast<UBoxComponent>(fire->GetComponentByClass(UBoxComponent::StaticClass()));
		float minusZ = fireBoxComp->GetScaledBoxExtent().Z;

		// (X,Y)가 (0,0)인 경우를 제외한 4 분면 위치 생성
		for (int i = 0; i < loopCountX; i++) {
			for (int j = 0; j < loopCountY; j++) {
				bool isStartPoint = i == 0 && j == 0;
				bool isFirstLine = i > 0 && j > 0;
				if (!isStartPoint) {
					if (isFirstLine) {
						BuildableLocations.Add(FVector(
							fire->GetActorLocation().X + (i * oneUnit),
							fire->GetActorLocation().Y + (j * oneUnit),
							fire->GetActorLocation().Z - minusZ
						));
						BuildableLocations.Add(FVector(
							fire->GetActorLocation().X - (i * oneUnit),
							fire->GetActorLocation().Y - (j * oneUnit),
							fire->GetActorLocation().Z - minusZ
						));
					}
					BuildableLocations.Add(FVector(
						fire->GetActorLocation().X + (i * oneUnit),
						fire->GetActorLocation().Y - (j * oneUnit),
						fire->GetActorLocation().Z - minusZ
					));
					BuildableLocations.Add(FVector(
						fire->GetActorLocation().X - (i * oneUnit),
						fire->GetActorLocation().Y + (j * oneUnit),
						fire->GetActorLocation().Z - minusZ
					));
				}
			}
			if (i >= FMath::Floor(loopCountX * 0.5f)) loopCountY -= 1;
		}
	}

	// 이미 건물이 있는 위치 제거
	for (int i = 0; i < BuildableLocations.Num(); i++) {
		for (auto building : buildingM->BuildingArr) {
			if (building->GetActorLocation().X == BuildableLocations[i].X && building->GetActorLocation().Y == BuildableLocations[i].Y) {
				BuildableLocations.RemoveAt(i);
				i--;
			}
		}
	}

	// tile 생성
	if (!BuildableISM->IsVisible()) BuildableISM->SetVisibility(true);
	if (BuildableISM->GetInstanceCount() == 0) BuildableISM->ClearInstances();
	for (auto location : BuildableLocations) {
		BuildableISM->AddInstance(FTransform(
			FRotator().ZeroRotator,
			location,
			FVector(TileScale)
		));
	}
}

void ATileManager::OffBuildableTile()
{
	BuildableISM->ClearInstances();
}


