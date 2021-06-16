// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainCameraPawn.generated.h"

UCLASS()
class ABANDONEDCAMP_API AMainCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainCameraPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* Sphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UFloatingPawnMovement* PawnMovementComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void TransportToStartPoint();

	// Translate
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pawn")
	float TurnSpeed = 0.1f;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnRight(float Value);


	// CurrentTile checking
	void CheckCurrentTile();
	void HoverActorWithTag(TArray<FHitResult> OutHits, FName Tag);


	// Touch interaction
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bCanSelectTile = true;

	UFUNCTION()
	void CallLeftClickEvent();

	UFUNCTION(Server, Reliable)
	void Server_TouchActor(FName ActorName, FVector ActorLocation);
	void Server_TouchActor_Implementation(FName ActorName, FVector ActorLocation);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_TouchActor(FName ActorName, FVector ActorLocation);
	void NetMulticast_TouchActor_Implementation(FName ActorName, FVector ActorLocation);

private:
	AController* HitController;
	TArray<FHitResult> TraceCursor();
};
