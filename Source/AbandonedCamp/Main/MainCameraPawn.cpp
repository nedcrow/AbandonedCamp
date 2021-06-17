// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCameraPawn.h"
#include "MainGS.h"
#include "MainPC.h"
#include "TileSystem/StartPointTile.h"
#include "TileSystem/TileManager.h"

#include "Camera/CameraComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"



// Sets default values
AMainCameraPawn::AMainCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<USphereComponent>(TEXT("Body"));
	RootComponent = Body;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Body);
	SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	PawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMovement"));
	
	bUseControllerRotationYaw = true;

}

// Called when the game starts or when spawned
void AMainCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	TransportToStartPoint();
}

// Called every frame
void AMainCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckCurrentTile();
}

// Called to bind functionality to input
void AMainCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainCameraPawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainCameraPawn::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("TurnRight"), this, &AMainCameraPawn::TurnRight);
	PlayerInputComponent->BindAction(TEXT("ClickLeft"), IE_Pressed, this, &AMainCameraPawn::CallLeftClickEvent);
}

void AMainCameraPawn::TransportToStartPoint()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AStartPointTile::StaticClass(), FName("StartPoint"), OutActors);
	if (OutActors.Num() > 0) {
		RootComponent->SetRelativeLocation(FVector(OutActors[0]->GetActorLocation().X, OutActors[0]->GetActorLocation().Y, 200.0f));
	}
	else {
		RootComponent->SetRelativeLocation(FVector(.0f, .0f, 200.0f));
	}
	if (RootComponent->GetRelativeRotation().Yaw != 0) {
		AddControllerYawInput((GetControlRotation().Yaw < 0 ? GetControlRotation().Yaw : -GetControlRotation().Yaw) * 2);
	}
}

void AMainCameraPawn::MoveForward(float Value)
{
	if (Value == 0)
	{
		return;
	}

	FVector ControlForwardVector2D = GetControlRotation().Vector().GetSafeNormal2D();
	AddMovementInput(ControlForwardVector2D, Value);
}

void AMainCameraPawn::MoveRight(float Value)
{
	if (Value == 0)
	{
		return;
	}

	FVector ControlRightVector2D = GetControlRotation().RotateVector(FVector::RightVector).GetSafeNormal2D();
	AddMovementInput(ControlRightVector2D, Value);
}

void AMainCameraPawn::TurnRight(float Value)
{
	if (Value == 0)
	{
		return;
	}

	AddControllerYawInput(Value * TurnSpeed);
}

void AMainCameraPawn::CheckCurrentTile(){
	TArray<FHitResult> outHits = TraceCursor();
	HoverActorWithTag(outHits, FName("Tile"));
}

void AMainCameraPawn::HoverActorWithTag(TArray<FHitResult> OutHits, FName Tag)
{
	/* 맞은 타일 확인 및 인덱스 번호 갱신 */
	AMainGS* gs = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	bool isFound = false;
	for (auto hit : OutHits) {
		if (hit.GetActor() && hit.GetActor()->GetComponentsByTag(UInstancedStaticMeshComponent::StaticClass(), Tag).Num() > 0) {
			UInstancedStaticMeshComponent* TileISM = Cast<UInstancedStaticMeshComponent>(hit.GetActor()->GetComponentsByTag(UInstancedStaticMeshComponent::StaticClass(), FName("Tile"))[0]);

			float closestValue = -1;
			int targetIndex = 0;
			FVector hitLocation = FVector(hit.ImpactPoint.X, hit.ImpactPoint.Y, 0);

			for (auto body : TileISM->InstanceBodies)
			{
				float dist = FVector::Dist(hitLocation, FVector(body->GetUnrealWorldTransform().GetLocation().X, body->GetUnrealWorldTransform().GetLocation().Y, 0));

				if (dist < closestValue || closestValue < 0) {
					closestValue = dist;
					targetIndex = body->InstanceBodyIndex;
				}
			}

			if (gs->HoveredTileIndex != targetIndex) {
				gs->HoveredTileIndex = targetIndex;
				gs->OnRep_ChangedTileIndex();
			}
			isFound = true;
			break;
		}
	}

	if (isFound == false && gs->HoveredTileIndex != -1) {
		gs->HoveredTileIndex = -1;
		gs->OnRep_ChangedTileIndex();
	}
}

void AMainCameraPawn::CallLeftClickEvent() {
	AMainGS* gs = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (gs) {
		TArray<FHitResult> outHits = TraceCursor();
		for (auto outHit : outHits) {
			if (outHit.GetActor() && outHit.GetActor()->ActorHasTag("Building"))
			{
				Server_TouchActor(outHit.GetActor()->GetFName(), outHit.GetActor()->GetActorLocation());
				break;
			}
		}

		AMainPC* pc = Cast<AMainPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (pc && gs->CurrentSelectedBuilding != nullptr) {
			pc->DeformateToLandscapeFrom(gs->CurrentSelectedBuilding);
		}

		ATileManager* TM = gs->GetTileManager();
		if (TM) {
			TM->OffBuildableTile();
			gs->CurrentSelectedBuilding = nullptr;
		}
	}
	
}

void AMainCameraPawn::Server_TouchActor_Implementation(FName ActorName, FVector ActorLocation) {
	NetMulticast_TouchActor(ActorName, ActorLocation);
}

void AMainCameraPawn::NetMulticast_TouchActor_Implementation(FName ActorName, FVector ActorLocation) {
	AMainGS* gs = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	gs->CurrentActorName = ActorName;
	gs->CurrentActorLocation = ActorLocation;
	gs->OnRep_ChangedCurrentActorName();
}

TArray<FHitResult> AMainCameraPawn::TraceCursor()
{
	TArray<FHitResult> OutHits;

	if (!bCanSelectTile) {
		return OutHits;
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && PC->IsLocalController()) {

		// tracing 준비
		float MouseX;
		float MouseY;
		FVector CursorWorldPosition;
		FVector CursorWorldDirection;

		FVector CameraLocation;
		FRotator CameraRotation;

		PC->GetMousePosition(MouseX, MouseY);
		PC->DeprojectScreenPositionToWorld(MouseX, MouseY, CursorWorldPosition, CursorWorldDirection);

		PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

		FVector TraceStart = CameraLocation;
		FVector TraceEnd = TraceStart + (CursorWorldDirection * 99999.f);

		TArray<TEnumAsByte<EObjectTypeQuery>> Objects;
		Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

		TArray<AActor*> ActorToIgnore;

		// 장애물을 대비한 multi tracing
		bool Result = UKismetSystemLibrary::LineTraceMultiForObjects(
			GetWorld(),
			TraceStart,
			TraceEnd,
			Objects,
			true,
			ActorToIgnore,
			EDrawDebugTrace::None,
			OutHits,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			100.0f
		);
		return OutHits;
	}
	return OutHits;
}

