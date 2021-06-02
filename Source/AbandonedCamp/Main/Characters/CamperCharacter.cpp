// Fill out your copyright notice in the Description page of Project Settings.


#include "CamperCharacter.h"
#include "../AI/CamperAIController.h"
#include "../BuildingManager.h"

//#include "BehaviorTree/BlackboardComponent.h"
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

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon2"));
	Weapon->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->bOnlySensePlayers = false;
	PawnSensing->bHearNoises = false;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
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

void ACamperCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ACamperCharacter, CurrentState);
}

void ACamperCharacter::OnRep_CurrentState()
{
}

// CurrentState가 Idle에서 바뀌지 않으면 타겟을 변경
void ACamperCharacter::SetCurrentState(ECharacterState NewState)
{
	ACamperAIController* AIC = GetController<ACamperAIController>();
	if (AIC)
	{
		// 타겟 주변 Nm 안으로 순찰 위치 변경
		if (NewState == ECharacterState::Idle) {
			ABuildingManager* buildingM = Cast<ABuildingManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABuildingManager::StaticClass()));
			if (buildingM && buildingM->FireBuildingArr.Num() > 0) {
				int max = buildingM->FireBuildingArr.Num() - 1;
				int index = FMath::RandRange(0, max);
				FVector targetLocation = buildingM->FireBuildingArr[index]->GetActorLocation();
				targetLocation = FVector(
					targetLocation.X + (FMath::RandRange(100, 300) * (FMath::RandBool() ? -1 : 1)),
					targetLocation.Y + (FMath::RandRange(100, 300) * (FMath::RandBool() ? -1 : 1)),
					GetActorLocation().Z
				);

				AIC->SetTargetActor(buildingM->FireBuildingArr[index]);
				AIC->SetTargetLocation(targetLocation);
			}
			return;
		}
		
		// 상태 변경
		/*CurrentState = NewState;
		AIC->SetCurrentState(NewState);*/
	}
}

void ACamperCharacter::ProcessSeenPawn(APawn* Pawn)
{
	if (CurrentState == ECharacterState::Idle) {
		
		if (Pawn->ActorHasTag("Stranger")) {
			ACamperAIController* AIC = GetController<ACamperAIController>();
			if (AIC && AIC->CurrentEnermy != Pawn)
			{
				AIC->SetTargetActor(Pawn);
				AIC->SetTargetLocation(Pawn->GetActorLocation());

				UE_LOG(LogTemp, Warning, TEXT("I found you [%s] :-)"), *Pawn->GetFName().ToString());
			}
		}
	}
}

