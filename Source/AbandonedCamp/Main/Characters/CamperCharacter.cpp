// Fill out your copyright notice in the Description page of Project Settings.


#include "CamperCharacter.h"
#include "../AI/CamperAIController.h"
#include "../UI/HUDSceneComponent.h"
#include "../UI/HPBarWidgetBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
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
	Weapon->OnComponentBeginOverlap.AddDynamic(this, &ACamperCharacter::OnBeginOverlap);
	WeaponArr.Add(Weapon);

	HUDScene = CreateDefaultSubobject<UHUDSceneComponent>(TEXT("HUDScene"));
	HUDScene->SetupAttachment(RootComponent);
	HUDScene->SetRelativeLocation(FVector(0.f, 0.f, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));
	HPBarWidget->SetupAttachment(HUDScene);
	HPBarWidget->SetRelativeRotation(FRotator(0, 180, 0));
	HPBarWidget->SetDrawSize(FVector2D(80.0f, 12.0f));

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
	OnRep_CurrentHP();

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &ACamperCharacter::ProcessSeenPawn);
	}
}

void ACamperCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACamperCharacter, CurrentHP);
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
		UE_LOG(LogTemp, Warning, TEXT("OverlapEvent - 1"));
		if (bCanAttack) {
			UE_LOG(LogTemp, Warning, TEXT("OverlapEvent - 2"));
			if (GetWorld()->IsServer()) {
				UGameplayStatics::ApplyPointDamage(OtherActor, AttackPoint, SweepResult.ImpactNormal, SweepResult, GetController(), this, UDamageType::StaticClass());
				UE_LOG(LogTemp, Warning, TEXT("OverlapEvent: to %s"), *OtherActor->GetFName().ToString());
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
	}

	if (CurrentHP <= 0) {
		ACamperAIController* AIC = GetController<ACamperAIController>();
		if (AIC) AIC->SetCurrentState(ECharacterState::Dead);
	}

	return 0.0f;
}

void ACamperCharacter::OnRep_CurrentHP()
{
	UHPBarWidgetBase* HPBarWidgetObj = Cast<UHPBarWidgetBase>(HPBarWidget->GetUserWidgetObject());
	if (HPBarWidgetObj)
	{
		HPBarWidgetObj->SetHPBar(CurrentHP / MaxHP);
	}
}

