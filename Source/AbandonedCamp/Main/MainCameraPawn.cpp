// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCameraPawn.h"
#include "BuildingManager.h"
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

void AMainCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	TransportToStartPoint();
}

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
		RootComponent->SetRelativeLocation(FVector(.0f, .0f, 200.0f));
		for (auto actor : OutActors) {
			if (actor->ActorHasTag(TEXT("Camper"))) {
				RootComponent->SetRelativeLocation(FVector(actor->GetActorLocation().X, actor->GetActorLocation().Y, 200.0f));
				break;
			}
		}
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

	/* 맞은 타일 확인 및 GS->HoveredTileIndex 갱신 */
	AMainGS* GS = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	bool isFound = false;
	for (auto hit : outHits) {
		if (hit.GetActor() && hit.GetActor()->GetComponentsByTag(UInstancedStaticMeshComponent::StaticClass(), FName("Tile")).Num() > 0) {
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

			if (GS->HoveredTileIndex != targetIndex) {
				GS->HoveredTileIndex = targetIndex;
				GS->OnRep_ChangedTileIndex();
			}
			isFound = true;
			break;
		}
	}

	if (isFound == false && GS->HoveredTileIndex != -1) {
		GS->HoveredTileIndex = -1;
		GS->OnRep_ChangedTileIndex();
	}
}

void AMainCameraPawn::CallLeftClickEvent() {
	AMainGS* GS = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS && GS->CurrentUIState != EUIState::Build) {
		// Touch Actor
		TArray<FHitResult> outHits = TraceCursor();
		for (auto outHit : outHits) {
			if (outHit.GetActor() && outHit.GetActor()->ActorHasTag("Building"))
			{
				Server_TouchActor(outHit.GetActor()->GetFName(), outHit.GetActor()->GetActorLocation());
				break;
			}
		}		
	}	

	Build();
}

void AMainCameraPawn::Server_TouchActor_Implementation(FName ActorName, FVector ActorLocation) {
	NetMulticast_TouchActor(ActorName, ActorLocation);
}

void AMainCameraPawn::NetMulticast_TouchActor_Implementation(FName ActorName, FVector ActorLocation) {
	AMainGS* GS = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	GS->CurrentActorName = ActorName;
	GS->CurrentActorLocation = ActorLocation;
	GS->OnRep_ChangedCurrentActorName();
}

void AMainCameraPawn::Build()
{
	AMainGS* GS = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS) {
		// 건물 위치가 모닥불 근처인지 확인
		bool isBuildable = false;
		ATileManager* TM = GS->GetTileManager();
		if (TM) {
			for (auto location : TM->BuildableLocations) {
				if (GS->CurrentSelectedBuilding) {
					if (location.X == GS->CurrentSelectedBuilding->GetActorLocation().X && location.Y == GS->CurrentSelectedBuilding->GetActorLocation().Y) {
						isBuildable = true;
						break;
					}
				}
			}

			// 건설 모드 종료
			if (isBuildable) {
				// 새 건물 건설 시 DeformateToLandscape
				AMainPC* PC = Cast<AMainPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
				if (PC && GS->CurrentSelectedBuilding != nullptr) {
					PC->DeformateToLandscapeFrom(GS->CurrentSelectedBuilding);
				}

				// BM 업데이트
				ABuildingManager* BM = ABuildingManager::GetInstance();
				BM->UpdateManager();

				// TM, GS 초기화
				TM->OffBuildableTile();
				GS->CurrentSelectedBuilding = nullptr;
				GS->CurrentUIState = EUIState::Normal;

				// Build 알림
				GS->CallBuildingEvent();
			}
		}
	}
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
		Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
		Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));

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

