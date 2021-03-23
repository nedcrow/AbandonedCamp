// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCameraPawn.h"
#include "MainGS.h"
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
	Body->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Body);
	SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	PawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMovement"));
	PawnMovementComponent->UpdatedComponent = RootComponent;

	bUseControllerRotationYaw = true;

}

// Called when the game starts or when spawned
void AMainCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	RootComponent->SetRelativeLocation(FVector(.0f, .0f, 200.0f));
}

// Called every frame
void AMainCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SelectTile();
}

// Called to bind functionality to input
void AMainCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainCameraPawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainCameraPawn::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("TurnRight"), this, &AMainCameraPawn::TurnRight);
	PlayerInputComponent->BindAction(TEXT("ClickLeft"), IE_Pressed, this, &AMainCameraPawn::SelectTile);
}

void AMainCameraPawn::MoveForward(float Value)
{
	if (Value == 0)
	{
		return;
	}

	//AddMovementInput(GetActorForwardVector(), Value);
	//AddMovementInput(FVector::ForwardVector, Value);
	FVector ControlForwardVector2D = GetControlRotation().Vector().GetSafeNormal2D();
	AddMovementInput(ControlForwardVector2D, Value);
}

void AMainCameraPawn::MoveRight(float Value)
{
	if (Value == 0)
	{
		return;
	}

	//AddMovementInput(GetActorRightVector(), Value);
	//AddMovementInput(FVector::RightVector, Value);
	FVector ControlRightVector2D = GetControlRotation().RotateVector(FVector::RightVector).GetSafeNormal2D();
	AddMovementInput(ControlRightVector2D, Value);
}

void AMainCameraPawn::TurnRight(float Value)
{
	if (Value == 0)
	{
		return;
	}

	AddControllerYawInput(Value * bTurnSpeed);
}

void AMainCameraPawn::SelectTile()
{
	if (!bCanSelectTile) {
		return;
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC) {
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

		// -- 

		TArray<TEnumAsByte<EObjectTypeQuery>> Objects;

		Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

		TArray<AActor*> ActorToIgnore;

		FHitResult OutHit;

		bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			TraceStart,
			TraceEnd,
			Objects,
			true,
			ActorToIgnore,
			EDrawDebugTrace::None,
			OutHit,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			5.0f
		);

		if(OutHit.GetActor() && OutHit.GetActor()->GetComponentsByTag(UInstancedStaticMeshComponent::StaticClass(), FName("ISM")).Num() > 0) {
			UInstancedStaticMeshComponent* TileISM = Cast<UInstancedStaticMeshComponent>(OutHit.GetActor()->GetComponentsByTag(UInstancedStaticMeshComponent::StaticClass(), FName("ISM"))[0]);
			
			float closestValue = -1;
			int targetIndex = 0;
			FVector hitLocation = FVector(OutHit.ImpactPoint.X, OutHit.ImpactPoint.Y, 0);

			for(auto body : TileISM->InstanceBodies)
			{
				float dist = FVector::Dist(hitLocation, FVector(body->GetUnrealWorldTransform().GetLocation().X, body->GetUnrealWorldTransform().GetLocation().Y, 0));

				if(dist < closestValue || closestValue < 0) { 
					closestValue = dist; 
					targetIndex = body->InstanceBodyIndex;
				}
			}
			/*UE_LOG(LogTemp, Warning, TEXT("Hit - (%f, %f)"), OutHit.ImpactPoint.X, OutHit.ImpactPoint.Y);
			UE_LOG(LogTemp, Warning, TEXT("Tile Location: (%f, %f)"), TileISM->InstanceBodies[0]->GetUnrealWorldTransform().GetLocation().X, TileISM->InstanceBodies[0]->GetUnrealWorldTransform().GetLocation().Y);
			UE_LOG(LogTemp, Warning, TEXT("Tile Index : %d"), targetIndex);*/
			AMainGS* gs = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
			gs->hoveredTileIndex = targetIndex;
		}
		else {
			 AMainGS* gs = Cast<AMainGS>(UGameplayStatics::GetGameState(GetWorld()));
			 gs->hoveredTileIndex = -1;
		}
		//if (OutHit.GetActor()->ActorHasTag("Tile"))	UE_LOG(LogTemp, Warning, TEXT("(hit)"));
	}
}

