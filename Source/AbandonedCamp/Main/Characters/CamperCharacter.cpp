// Fill out your copyright notice in the Description page of Project Settings.


#include "CamperCharacter.h"
#include "../AI/CamperAIController.h"
#include "../BuildingManager.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ACamperCharacter::ACamperCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	Weapon->SetGenerateOverlapEvents(true);

	//WeaponCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCapsule"));
	//WeaponCapsule->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	//WeaponCapsule->SetGenerateOverlapEvents(true);
	Weapon->OnComponentBeginOverlap.AddDynamic(this, &ACamperCharacter::OnBeginOverlap);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->bOnlySensePlayers = false;
	PawnSensing->bHearNoises = false;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CountAttackAnim = 3;
	CountHitAnim = 3;
}

// Called when the game starts or when spawned
void ACamperCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetCurrentState(ECharacterState::Idle);
	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &ACamperCharacter::ProcessSeenPawn);
	}
}

// Called every frame
void ACamperCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACamperCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACamperCharacter::ProcessSeenPawn(APawn* Pawn)
{
	if (CurrentState != ECharacterState::Dead) {
		
		if (Pawn->ActorHasTag("Stranger")) {
			ACamperAIController* AIC = GetController<ACamperAIController>();
			if (AIC && AIC->CurrentEnermy != Pawn)
			{
				AIC->SetTargetActor(Pawn);
				AIC->SetTargetLocation(Pawn->GetActorLocation());
				AIC->SetCurrentState(ECharacterState::Guard);
			}
		}
	}
}

void ACamperCharacter::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult) {
	bool isSuccess = OtherActor->ActorHasTag(TEXT("Stranger"));

	if (isSuccess) {
		if (bCanAttack) {
			UE_LOG(LogTemp, Warning, TEXT("OverlappedComponent: %s"), *OverlappedComponent->GetFName().ToString());
			UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s"), *OtherActor->GetFName().ToString());
			UE_LOG(LogTemp, Warning, TEXT("OtherComp: %s"), *OtherComp->GetFName().ToString());
			UE_LOG(LogTemp, Warning, TEXT("OtherBodyIndex: %d"), OtherBodyIndex);
			UE_LOG(LogTemp, Warning, TEXT("bFromSweep: %s"), bFromSweep ? TEXT("True") : TEXT("False"));

			bCanAttack = false;
		}
	}
	
	
}

