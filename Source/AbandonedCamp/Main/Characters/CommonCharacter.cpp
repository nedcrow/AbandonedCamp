// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonCharacter.h"
#include "../AI/CommonAIController.h"

// Sets default values
ACommonCharacter::ACommonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACommonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACommonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACommonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACommonCharacter::SetCurrentState(ECharacterState NewState)
{
	ACommonAIController* AIC = GetController<ACommonAIController>();
	if (AIC)
	{
		CurrentState = NewState;
		AIC->SetCurrentState(NewState);
	}
}

void ACommonCharacter::SetTargetActor(AActor* TargetActor)
{
	ACommonAIController* AIC = GetController<ACommonAIController>();
	if (AIC)
	{
		CurrentTargetActor = TargetActor;
		AIC->SetTargetActor(TargetActor);
	}
}

void ACommonCharacter::SetGenerateOverlapEventOfWeapons(bool Value)
{
	for (auto weapon : WeaponArr) {
		weapon->SetGenerateOverlapEvents(Value);
	}

	for (auto weaponSphere : WeaponSphereArr) {
		weaponSphere->SetGenerateOverlapEvents(Value);
	}
}

