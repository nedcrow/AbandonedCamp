// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingTile.h"
#include "../MainGS.h"
#include "../UI/HUDSceneComponent.h"
#include "../UI/OnOffButtonWidgetBase.h"
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

void ABuildingTile::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();
}

void ABuildingTile::BeginPlay()
{
	Super::BeginPlay();
	AMainGS* gs = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (gs) gs->F_TouchEvent.AddUFunction(this, FName("CallDelFunc_TouchEvent"));
}

void ABuildingTile::SpawnEffects()
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

void ABuildingTile::DestroyEffects()
{
	for (auto effectComponent : InteractionEffectsComponents) {
		effectComponent->DestroyComponent();
	}

	InteractionEffectsComponents.Empty();
}

void ABuildingTile::CallDelFunc_TouchEvent(FName TargetName, FVector TargetLocation)
{
	bool canTouch = TargetName == GetFName() &&
		TargetLocation == GetActorLocation() &&
		bCanInteraction == true;

	if (bCanInteraction) {
		if (bIsTouched) {
			DestroyEffects();
			bIsTouched = false;
		}
		else {
			SpawnEffects();
			bIsTouched = true;
		}
	}
}