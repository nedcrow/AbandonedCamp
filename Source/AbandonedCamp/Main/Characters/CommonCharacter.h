// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "CommonCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle	UMETA(DisplayName = "Idle"),
	ToFlame	UMETA(DisplayName = "ToFlame"),
	Guard	UMETA(DisplayName = "Guard"),
	Battle	UMETA(DisplayName = "Battle"),
	Dead	UMETA(DisplayName = "Dead")
};

UCLASS()
class ABANDONEDCAMP_API ACommonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACommonCharacter();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UHUDSceneComponent* HUDScene;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UWidgetComponent* HPBarWidget;

protected:	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Status
	UPROPERTY(ReplicatedUsing = "OnRep_CurrentHP", BlueprintReadWrite, EditAnywhere, Category = "Status")
	float CurrentHP = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float MaxHP = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float WalkSpeed = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float RunSpeed = 200.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float WarmthSight = 1000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float AttackPoint = 30.0f;

	UFUNCTION()
	void OnRep_CurrentHP();

	// AI
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
	ECharacterState CurrentState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
	AActor* CurrentTargetActor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim")
	int CountAttackAnim = 3;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim")
	int CountHitAnim = 3;	

	UFUNCTION(BlueprintCallable)
	void SetCurrentState(ECharacterState NewState);

	UFUNCTION(BlueprintCallable)
	void SetTargetActor(AActor* TargetActor);

	// Attack
	TArray<UStaticMeshComponent*> WeaponArr;
	TArray<USphereComponent*> WeaponSphereArr;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attack")
	bool bCanAttack = false;

	void SetGenerateOverlapEventOfWeapons(bool Value);
};
