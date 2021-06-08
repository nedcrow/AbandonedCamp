// Fill out your copyright notice in the Description page of Project Settings.


#include "StrangerCharacter.h"
#include "../AI/StrangerAIController.h"
#include "../BuildingManager.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AStrangerCharacter::AStrangerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	CountAttackAnim = 2;
	CountHitAnim = 2;
}

// Called when the game starts or when spawned
void AStrangerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AStrangerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AStrangerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AStrangerCharacter::ProcessSeenPawn(APawn* Pawn)
{
	if (CurrentState != ECharacterState::Dead) {

		if (Pawn->ActorHasTag("Camper")) {
			/*ACamperAIController* AIC = GetController<ACamperAIController>();
			if (AIC && AIC->CurrentEnermy != Pawn)
			{
				AIC->SetTargetActor(Pawn);
				AIC->SetTargetLocation(Pawn->GetActorLocation());

				UE_LOG(LogTemp, Warning, TEXT("I found you [%s] :-)"), *Pawn->GetFName().ToString());
			}*/
		}
	}
}

