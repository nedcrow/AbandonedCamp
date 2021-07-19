// Fill out your copyright notice in the Description page of Project Settings.


#include "StrangerCharacter.h"
#include "../AI/StrangerAIController.h"
#include "../MainGS.h"
#include "../UI/HPBarWidgetBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
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

	WeaponA = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponA"));
	WeaponA->SetupAttachment(GetMesh(), TEXT("WeaponSocketA"));
	WeaponA->OnComponentBeginOverlap.AddDynamic(this, &AStrangerCharacter::OnBeginOverlap);
	WeaponA->SetGenerateOverlapEvents(false);
	WeaponSphereArr.Add(WeaponA);

	WeaponB = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponB"));
	WeaponB->SetupAttachment(GetMesh(), TEXT("WeaponSocketB"));
	WeaponB->OnComponentBeginOverlap.AddDynamic(this, &AStrangerCharacter::OnBeginOverlap);
	WeaponB->SetGenerateOverlapEvents(false);
	WeaponSphereArr.Add(WeaponB);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	// Status
	CurrentHP = MaxHP = 100.0f;
	AttackPoint = 5.0f;
	WalkSpeed = 100.0f;
	RunSpeed = 200.0f;

	CountAttackAnim = 2;
	CountHitAnim = 2;

	Tags.Add(TEXT("Stranger"));

}

// Called when the game starts or when spawned
void AStrangerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// HP
	OnRep_CurrentHP();
	if(HPBarWidget) {
		UHPBarWidgetBase* HPBarWidgetObj = Cast<UHPBarWidgetBase>(HPBarWidget->GetUserWidgetObject());
		HPBarWidgetObj->SetHPBarColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
	}


	// Bind events
	AMainGS* GS = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS) {
		GS->F_OnNightEvent.AddUFunction(this, FName("CallDelFunc_OnNightEvent"));
	}

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AStrangerCharacter::ProcessSeenPawn);
	}
}

// Called to bind functionality to input
void AStrangerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AStrangerCharacter::ProcessSeenPawn(APawn* Pawn)
{
	if (CurrentState != ECharacterState::Dead) {

		/*if (Pawn->ActorHasTag("Camper")) {
			AStrangerAIController* AIC = GetController<AStrangerAIController>();
			ACommonCharacter* CommonChar = Cast<ACommonCharacter>(Pawn);
			bool canSetEnermy = CommonChar->CurrentState != ECharacterState::Dead && AIC && AIC->CurrentEnermy != Pawn;
			if (canSetEnermy)
			{
				AIC->CurrentEnermy = Pawn;
				AIC->SetTargetActor(Pawn);
				AIC->SetTargetLocation(Pawn->GetActorLocation());
				AIC->SetCurrentState(ECharacterState::Guard);
			}
		}*/
	}
}

void AStrangerCharacter::CallDelFunc_OnNightEvent(ENightState NightState)
{
	AStrangerAIController* AIC = GetController<AStrangerAIController>();
	if (AIC) {
		AIC->SetNightState(NightState);
	}
}

void AStrangerCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AStrangerAIController* AIC = GetController<AStrangerAIController>();
	bool isSuccess = OtherActor->ActorHasTag(TEXT("Camper")) || OtherActor->ActorHasTag(TEXT("Building"));
	isSuccess = isSuccess && AIC && AIC->CurrentEnermy;

	if (isSuccess) {
		if (bCanAttack) {
			if (GetWorld()->IsServer()) {
				UGameplayStatics::ApplyPointDamage(OtherActor, AttackPoint, SweepResult.ImpactNormal, SweepResult, GetController(), this, UDamageType::StaticClass());
			}
			bCanAttack = false;
		}
	}
}

float AStrangerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0) {
		return 0.f;
	}

	if (!GetWorld()->IsServer() || DamageCauser->ActorHasTag("Stranger"))
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
		SetCurrentState(ECharacterState::Dead);
	}

	return 0.0f;
}

