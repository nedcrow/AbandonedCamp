// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"
#include "Characters/CommonCharacter.h"
#include "Characters/CamperCharacter.h"
#include "Characters/StrangerCharacter.h"
#include "TileSystem/StartPointTile.h"
#include "MainGM.h"
#include "MainGS.h"
#include "MainPC.h"
#include "UI/MainUIWidgetBase.h"
#include "UI/CamperListWidgetBase.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ASpawnManager* ASpawnManager::Instance_;

ASpawnManager* ASpawnManager::GetInstance()
{
	ASpawnManager* SM;
	if (Instance_) {
		SM = Instance_;
	}
	else {
		UWorld* world = GEngine->GameViewport->GetWorld();
		SM = Cast<ASpawnManager>(UGameplayStatics::GetActorOfClass(world, ASpawnManager::StaticClass()));
		if (!SM) {
			SM = world->SpawnActor<ASpawnManager>();
			#if WITH_EDITOR
				SM->SetFolderPath(TEXT("/Managers"));
			#endif // WITH_EDITOR
		}
	}
	return SM;
}

ASpawnManager::ASpawnManager()
{
	PrimaryActorTick.bCanEverTick = true;	
	static ConstructorHelpers::FClassFinder<ACamperCharacter> BP_Camper(TEXT("Blueprint'/Game/BluePrints/Main/Characters/BP_CamperCharacter'"));
	CamperActor = BP_Camper.Class;
	static ConstructorHelpers::FClassFinder<AStrangerCharacter> BP_Stranger(TEXT("Blueprint'/Game/BluePrints/Main/Characters/BP_StrangerCharacter'"));
	StrangerActor = BP_Stranger.Class;
}

void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();

	// 새 액터 생성 및 분할 업데이트
	AMainGM* GM = Cast<AMainGM>(UGameplayStatics::GetGameMode(GetWorld()));

	// Camper
	int countOfCamper = FMath::Abs(GM->CamperCount - SpawnedCamperActors.Num());
	for (int i = 0; i < countOfCamper; i++) {
		SpawnActorThat(ESpawnableType::Camper);
	}
	for (int i = 0; i < SpawnedCamperActors.Num(); i++) {
		FString tempName = "Camper_" + FString::FromInt(i);
		Cast<ACamperCharacter>(SpawnedCamperActors[i])->CamperName = FName(*tempName);
	}
	AMainPC* PC = Cast<AMainPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC->MainUIWidgetObject) PC->MainUIWidgetObject->CamperListWidget->UpdateList();

	// Stranger
	int countOfStranger = FMath::Abs(GM->StrangerCount - SpawnedStrangerActors.Num());
	for (int i = 0; i < countOfStranger; i++) {
		SpawnActorThat(ESpawnableType::Stranger);
	}
}

void ASpawnManager::UpdateSpawnedActors()
{
	// 초기화
	SpawnedCamperActors.Empty();
	SpawnedStrangerActors.Empty();

	// 기존 액터 분할 업데이트
	TArray<AActor*> outActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), outActors);
	for (auto character : outActors) {
		if (character->ActorHasTag(TEXT("Camper"))) SpawnedCamperActors.Add(character);
		if (character->ActorHasTag(TEXT("Stranger"))) SpawnedStrangerActors.Add(character);
	}
}

void ASpawnManager::SpawnActorThat(ESpawnableType Type, ACommonCharacter* TargetActor)
{
	TArray<AActor*> startPointActors;
	FName startPointTag;

	switch (Type)
	{
	case ESpawnableType::Camper:
		startPointTag = TEXT("Camper");
		if(TargetActor != nullptr) break;
		if (CamperActor) {
			TargetActor = GetWorld()->SpawnActor<ACamperCharacter>(CamperActor, FVector(0.f, 0.f, -999.f), FRotator().ZeroRotator);				
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Error: None CamperActor <CamperCharacter*> (or blueprint)"));
		}
		break;
	case ESpawnableType::Stranger:
		startPointTag = TEXT("Stranger");
		if (TargetActor != nullptr) break;
		if (StrangerActor) {
			TargetActor = GetWorld()->SpawnActor<AStrangerCharacter>(StrangerActor, FVector(0.f, 0.f, -999.f), FRotator().ZeroRotator);				
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Error: None CamperActor <StrangerCharacter*> (or blueprint)"));
		}
		break;
	default:
		break;
	}

	// 사용 가능한 startPointActor 위치만 참고하여 배치
	if (TargetActor != nullptr) {
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AStartPointTile::StaticClass(), startPointTag, startPointActors);
		if (startPointActors.Num() > 0) {
			int randX = FMath::RandRange(60, 200);
			int randY = FMath::RandRange(60, 200);
			TArray<FVector> tempLocationArr;
			for (auto actor : startPointActors) {
				AStartPointTile* startPoint = Cast<AStartPointTile>(actor);
				if (startPoint->bUseablePoint == true) {
					tempLocationArr.Add(FVector(
						startPoint->GetActorLocation().X + ((FMath::RandRange(0, 1) == 0 ? 1 : -1) * randX),
						startPoint->GetActorLocation().Y + ((FMath::RandRange(0, 1) == 0 ? 1 : -1) * randY),
						startPoint->GetActorLocation().Z)
						);
				}
			}
			FVector startPointLocation = startPointActors.Num() > 0 ? tempLocationArr[FMath::RandRange(0, tempLocationArr.Num()-1)] : FVector::ZeroVector;
			TargetActor->SetActorLocation(startPointLocation);
			TargetActor->CurrentHP = TargetActor->MaxHP;
			TargetActor->OnRep_CurrentHP();
			TargetActor->SetCurrentState(ECharacterState::Idle);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Error: Not found start point of %s"), *startPointTag.ToString());
		}
	}

	UpdateSpawnedActors();
}
