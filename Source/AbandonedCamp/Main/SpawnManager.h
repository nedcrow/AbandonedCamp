// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/CommonCharacter.h"
#include "Characters/CamperCharacter.h"
#include "Characters/StrangerCharacter.h"
#include "SpawnManager.generated.h"

UENUM(BlueprintType)
enum class ESpawnableType : uint8
{
	Camper		UMETA(DisplayName = "Camper"),
	Stranger	UMETA(DisplayName = "Stranger"),
};

UCLASS()
class ABANDONEDCAMP_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class ACamperCharacter> CamperActor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class AStrangerCharacter> StrangerActor;

	TArray<AActor*> SpawnedCamperActors;
	TArray<AActor*> SpawnedStrangerActors;

	UFUNCTION()
	void UpdateSpawnedActors();

	void SpawnActorThat(ESpawnableType Type, ACommonCharacter* TargetActor);
};
