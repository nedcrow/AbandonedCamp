// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingTile.h"
#include "../UI/HUDSceneComponent.h"
#include "../UI/OnOffButtonWidgetBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ABuildingTile::ABuildingTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// RootComponent 생성
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(50.0f));

	// BodyMesh 생성 후 위치를 Box 바닥으로 이동
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->AddRelativeLocation(FVector(0.f, 0.f, -Box->GetScaledBoxExtent().Z));

	HUDScene = CreateDefaultSubobject<UHUDSceneComponent>(TEXT("HUDScene"));
	HUDScene->SetupAttachment(RootComponent);

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(HUDScene);
	Widget->SetRelativeRotation(FRotator(0, 180, 0));

	Tags.Add(FName("Building"));
}

void ABuildingTile::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();
}

float ABuildingTile::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bCanInteraction) {
		if (bIsTouched) {
			NetMulticast_DestroyEffects();
			bIsTouched = false;
		}
		else {
			NetMulticast_SpawnEffects();
			bIsTouched = true;
		}

	}

	return 0.0f;
}

void ABuildingTile::NetMulticast_SpawnEffects_Implementation()
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

void ABuildingTile::NetMulticast_DestroyEffects_Implementation()
{
	for (auto effectComponent : InteractionEffectsComponents) {
		effectComponent->DestroyComponent();
	}

	InteractionEffectsComponents.Empty();
}
