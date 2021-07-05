// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingTile.h"
#include "BonFireComponent.h"
#include "../BuildingManager.h"
#include "../MainGS.h"
#include "../UI/HUDSceneComponent.h"
#include "../UI/HPBarWidgetBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"

ABuildingTile::ABuildingTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HUDScene = CreateDefaultSubobject<UHUDSceneComponent>(TEXT("HUDScene"));
	HUDScene->SetupAttachment(RootComponent);

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(HUDScene);
	Widget->SetRelativeRotation(FRotator(0, 180, 0));
	Widget->SetDrawSize(FVector2D(0.1f, 0.1f));

	Tags.Add(FName("Building"));
}

void ABuildingTile::BeginPlay()
{
	Super::BeginPlay();
	OnRep_CurrentHP();

	// Event
	AMainGS* GS = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS) GS->F_TouchEvent.AddUFunction(this, FName("CallDelFunc_TouchEvent"));
}

void ABuildingTile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABuildingTile, CurrentHP);
}

void ABuildingTile::SpawnInteractionEffects()
{
	if (InteractionEffects.Num() > 0 && InteractionEffectsComponents.Num() == 0) {
		for (auto effect : InteractionEffects) {
			InteractionEffectsComponents.Add(
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					effect,
					GetActorLocation()
				)
			);
		}
	}
}

void ABuildingTile::DestroyInteractionEffects()
{
	for (auto effectComponent : InteractionEffectsComponents) {
		effectComponent->DestroyComponent();
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeadEffect, GetActorLocation());
	InteractionEffectsComponents.Empty();
}

void ABuildingTile::CallDelFunc_TouchEvent(FName TargetName, FVector TargetLocation)
{
	bool canTouch = TargetName == GetFName() &&
		TargetLocation == GetActorLocation() &&
		bCanInteraction == true;

	if (bCanInteraction) {
		AMainGS* GS = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
		if (GS) {

			// Effect On/Off
			if (bIsTouched) {
				DestroyInteractionEffects();
				bIsTouched = false;
				GS->NightState = ENightState::Day;
				GS->OnRep_ChangedCurrentNight();
			}
			else {
				SpawnInteractionEffects();
				bIsTouched = true;
				GS->NightState = ENightState::Night;
				GS->OnRep_ChangedCurrentNight();

				// Plus round status
				if (bCanStartRound) {
					GS->CurrentNight++;
				}
			}
		}
	}
}

float ABuildingTile::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
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
		// particle
		DestroyInteractionEffects();

		// fireComp
		UBonFireComponent* myFireComp = Cast<UBonFireComponent>(GetComponentByClass(UBonFireComponent::StaticClass()));
		if(myFireComp && myFireComp->RenderTarget) myFireComp->ClearRenderTarget();

		// BuildingManager °»½Å
		AMainGS* GS = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
		if (GS) {
			ABuildingManager* BM = GS->GetBuildingManager();
			BM->BuildingArr.Remove(this);
			BM->FireBuildingArr.Remove(this);

			for (auto fireBuilding : BM->FireBuildingArr) {
				UBonFireComponent* fireComp = Cast<UBonFireComponent>(fireBuilding->GetComponentByClass(UBonFireComponent::StaticClass()));
				fireComp->DeformateToLandscape();
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Destroy Effect"));
		Destroy();
	}

	return 0.0f;
}

void ABuildingTile::OnRep_CurrentHP()
{
	UHPBarWidgetBase* HPBarWidgetObj = Cast<UHPBarWidgetBase>(Widget->GetUserWidgetObject());
	if (HPBarWidgetObj)
	{
		HPBarWidgetObj->SetHPBar(CurrentHP / MaxHP);
	}
}