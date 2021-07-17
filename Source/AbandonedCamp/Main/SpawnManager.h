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
	
private:
	static ASpawnManager* Instance_;
public:
	static ASpawnManager* GetInstance();
public:	
	ASpawnManager();

protected:
	virtual void BeginPlay() override;

public:	

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class ACamperCharacter> CamperActor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class AStrangerCharacter> StrangerActor;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<AActor*> SpawnedCamperActors;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<AActor*> SpawnedStrangerActors;

	UFUNCTION()
	void UpdateSpawnedActors();

	/* 
	* 액터 생성 및 스폰 된 액터 배열 갱신.
	* Type: 해당 타입의 액터 생성.
	* TargetActor: 기본값이 nullptr 경우 새로 생성.
	*/
	void SpawnActorThat(ESpawnableType Type, ACommonCharacter* TargetActor = nullptr);
};
