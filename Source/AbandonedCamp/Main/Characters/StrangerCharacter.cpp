// Fill out your copyright notice in the Description page of Project Settings.


#include "StrangerCharacter.h"
#include "../AI/StrangerAIController.h"
#include "../MainGS.h"
#include "../UI/HUDSceneComponent.h"
#include "../UI/HPBarWidgetBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
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

	HUDScene = CreateDefaultSubobject<UHUDSceneComponent>(TEXT("HUDScene"));
	HUDScene->SetupAttachment(RootComponent);
	HUDScene->SetRelativeLocation(FVector(0.f, 0.f, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));
	HPBarWidget->SetupAttachment(HUDScene);
	HPBarWidget->SetRelativeRotation(FRotator(0, 180, 0));
	HPBarWidget->SetDrawSize(FVector2D(80.0f, 12.0f));

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	CountAttackAnim = 2;
	CountHitAnim = 2;
	Tags.Add(TEXT("Stranger"));
}

// Called when the game starts or when spawned
void AStrangerCharacter::BeginPlay()
{
	Super::BeginPlay();
	OnRep_CurrentHP();

	// Event
	AMainGS* GS = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS) GS->F_OnNightEvent.AddUFunction(this, FName("CallDelFunc_OnNightEvent"));
}

void AStrangerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AStrangerCharacter, CurrentHP);
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
	isSuccess = isSuccess = isSuccess && AIC && AIC->CurrentEnermy;
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s"), *OtherActor->GetFName().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Overlap Success: %s"), isSuccess?TEXT("t") : TEXT("f"));

	if (isSuccess) {
		if (bCanAttack) {
			UE_LOG(LogTemp, Warning, TEXT("Successed OtherActor: %s"), *OtherActor->GetFName().ToString());
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
		SetCurrentState(ECharacterState::Dead);
	}

	return 0.0f;
}

void AStrangerCharacter::OnRep_CurrentHP()
{
	UHPBarWidgetBase* HPBarWidgetObj = Cast<UHPBarWidgetBase>(HPBarWidget->GetUserWidgetObject());
	if (HPBarWidgetObj)
	{
		HPBarWidgetObj->SetHPBar(CurrentHP / MaxHP);
	}
}

