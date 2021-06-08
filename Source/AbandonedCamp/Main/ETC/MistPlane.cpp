// Fill out your copyright notice in the Description page of Project Settings.


#include "MistPlane.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetRenderingLibrary.h"

// Sets default values
AMistPlane::AMistPlane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
}

void AMistPlane::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), RenderTarget);
	UE_LOG(LogTemp, Warning, TEXT("MistPlane"));
}
