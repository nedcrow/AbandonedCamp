// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"
#include "Characters/CommonCharacter.h"
#include "Characters/CamperCharacter.h"
#include "Characters/StrangerCharacter.h"
#include "TileSystem/StartPointTile.h"
#include "MainGS.h"
#include "MainGS.h"
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
}

void ASpawnManager::UpdateSpawnedActors()
{
	SpawnedCamperActors.Empty();
	SpawnedStrangerActors.Empty();
	TArray<AActor*> outActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), outActors);
	for (auto character : outActors) {
		if (character->ActorHasTag(TEXT("Camper"))) SpawnedCamperActors.Add(character);
		if (character->ActorHasTag(TEXT("Stranger"))) SpawnedStrangerActors.Add(character);
	}
}

void ASpawnManager::SpawnActorThat(ESpawnableType Type, ACommonCharacter* TargetActor = nullptr)
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

	if (TargetActor != nullptr) {
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AStartPointTile::StaticClass(), startPointTag, startPointActors);
		if (startPointActors.Num() > 0) {
			int randX = FMath::RandRange(60, 200);
			int randY = FMath::RandRange(60, 200);
			FVector startPointLocation = FVector(
				startPointActors[0]->GetActorLocation().X + ((FMath::RandRange(0, 1) == 0 ? 1 : -1) * randX),
				startPointActors[0]->GetActorLocation().Y + ((FMath::RandRange(0, 1) == 0 ? 1 : -1) * randY),
				startPointActors[0]->GetActorLocation().Z
			);
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
