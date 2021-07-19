// Fill out your copyright notice in the Description page of Project Settings.


#include "CamperCharacter.h"
#include "CommonCharacter.h"
#include "../MainPC.h"
#include "../AI/CamperAIController.h"
#include "../UI/MainUIWidgetBase.h"
#include "../UI/CamperListWidgetBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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
	Weapon->OnComponentBeginOverlap.AddDynamic(this, &ACamperCharacter::OnBeginOverlap);
	Weapon->SetGenerateOverlapEvents(false);
	WeaponArr.Add(Weapon);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->bOnlySensePlayers = false;
	PawnSensing->bHearNoises = false;

	// Status
	CurrentHP = MaxHP = 100.0f;
	AttackPoint = 30.0f;
	WalkSpeed = 150.0f;
	RunSpeed = 250.0f;
	CountAttackAnim = 3;
	CountHitAnim = 3;

	Tags.Add(TEXT("Camper"));
}

// Called when the game starts or when spawned
void ACamperCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetCurrentState(ECharacterState::Idle);
	OnRep_CurrentHP();

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &ACamperCharacter::ProcessSeenPawn);
	}
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
			ACommonCharacter* CommonChar = Cast<ACommonCharacter>(Pawn);
			bool canSetEnermy = CommonChar->CurrentState != ECharacterState::Dead && AIC && AIC->CurrentEnermy != Pawn;
			if (canSetEnermy)
			{
				AIC->WalkSpeed = WalkSpeed;
				AIC->CurrentEnermy = Pawn;
				AIC->SetTargetActor(Pawn);
				AIC->SetTargetLocation(Pawn->GetActorLocation());
				AIC->SetCurrentState(ECharacterState::Guard);
				GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
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

	ACamperAIController* AIC = GetController<ACamperAIController>();
	bool isSuccess = AIC && AIC->CurrentEnermy && OtherActor->ActorHasTag(TEXT("Stranger"));

	if (isSuccess) {
		if (bCanAttack) {
			if (GetWorld()->IsServer()) {
				UGameplayStatics::ApplyPointDamage(AIC->CurrentEnermy, AttackPoint, SweepResult.ImpactNormal, SweepResult, GetController(), this, UDamageType::StaticClass());
				bCanAttack = false;
			}
		}
	}	
}

float ACamperCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0) {
		return 0.f;
	}

	if (!GetWorld()->IsServer() || DamageCauser->ActorHasTag("Camper"))
	{
		return 0.f;
	}

	float tempHP = CurrentHP;
	tempHP -= DamageAmount;

	if (CurrentHP != tempHP)
	{
		CurrentHP = tempHP;
		OnRep_CurrentHP();
		AMainPC* PC = Cast<AMainPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PC) {
			PC->MainUIWidgetObject->CamperListWidget->UpdateList();
		}
	}

	if (CurrentHP <= 0) {
		ACamperAIController* AIC = GetController<ACamperAIController>();
		if (AIC) AIC->SetCurrentState(ECharacterState::Dead);
	}

	return 0.0f;
}

