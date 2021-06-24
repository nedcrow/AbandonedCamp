// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonCharacter.h"
#include "../AI/CommonAIController.h"
#include "../UI/HUDSceneComponent.h"
#include "../UI/HPBarWidgetBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACommonCharacter::ACommonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// UI
	HUDScene = CreateDefaultSubobject<UHUDSceneComponent>(TEXT("HUDScene"));
	HUDScene->SetupAttachment(RootComponent);
	HUDScene->SetRelativeLocation(FVector(0.f, 0.f, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));
	HPBarWidget->SetupAttachment(HUDScene);
	HPBarWidget->SetRelativeRotation(FRotator(0, 180, 0));
	HPBarWidget->SetDrawSize(FVector2D(80.0f, 12.0f));

	// AI
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ACommonCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACommonCharacter, CurrentHP);
}

// Called to bind functionality to input
void ACommonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACommonCharacter::OnRep_CurrentHP()
{
	UHPBarWidgetBase* HPBarWidgetObj = Cast<UHPBarWidgetBase>(HPBarWidget->GetUserWidgetObject());
	if (HPBarWidgetObj)
	{
		HPBarWidgetObj->SetHPBar(CurrentHP / MaxHP);
	}
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

