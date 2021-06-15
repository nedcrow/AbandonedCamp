// Fill out your copyright notice in the Description page of Project Settings.


#include "TileManager.h"
#include "BonFireComponent.h"
#include "../MainGS.h"
#include "../MainPC.h"
#include "../BuildingManager.h"

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

void ATileManager::OnBuildableTile()
{
	if (Able_Build_MI == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("!!!- Null : border_MI -!!!"));
		return;
	}


	/*for (int i=0; i< DefaultTileISM->GetInstanceCount(); i++)
	{
		FTransform temp;
		DefaultTileISM->GetInstanceTransform(i,temp);
		
		UE_LOG(LogTemp,Warning,TEXT("%f, %f, %f"), temp.GetLocation().X, temp.GetLocation().Y);
	}*/

	TArray<FVector> DecalLocations;
	ABuildingManager* buildingM = Cast<ABuildingManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABuildingManager::StaticClass()));
	for (auto fire : buildingM->FireBuildingArr) {
		UBonFireComponent* fireComponent = Cast<UBonFireComponent>(fire->GetComponentByClass(UBonFireComponent::StaticClass()));
		int loopCountX = fireComponent->FireLightRadius;
		int loopCountY = fireComponent->FireLightRadius;
		float oneUnit = 100 * TileScale;

		// (X,Y)가 (0,0)인 경우를 제외한 위치만 선택
		for (int i = 0; i < loopCountX; i++) {
			for (int j = 0; j < loopCountY; j++) {
				if (i == 0 && j == 0) {}
				else {
					DecalLocations.Add(FVector(
						fire->GetActorLocation().X + (i * oneUnit),
						fire->GetActorLocation().Y + (j * oneUnit),
						fire->GetActorLocation().Z
					));
					DecalLocations.Add(FVector(
						fire->GetActorLocation().X - (i * oneUnit),
						fire->GetActorLocation().Y - (j * oneUnit),
						fire->GetActorLocation().Z
					));
				}
			}
			loopCountY -= 1;
		}
	}

	for (auto location : DecalLocations) {
		if (!BuildableISM->IsVisible()) BuildableISM->SetVisibility(true);
		if (BuildableISM->GetInstanceCount() > 0) BuildableISM->ClearInstances();

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


