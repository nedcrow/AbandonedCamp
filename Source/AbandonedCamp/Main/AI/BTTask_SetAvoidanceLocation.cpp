// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetAvoidanceLocation.h"
#include "CamperAIController.h"

#include "BehaviorTree/BlackBoardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

EBTNodeResult::Type UBTTask_SetAvoidanceLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACamperAIController* AIC = Cast<ACamperAIController>(OwnerComp.GetAIOwner());
	FVector avoidanceLocation = AIC->BBComponent->GetValueAsVector(TEXT("TargetLocation"));
	//FVector avoidanceLocation = AIC->BBComponent->GetValueAsVector(TEXT("TargetLocation"));

	if (AIC && AIC->IsLocalController()) {
		FVector tempLocation = FindNoBlocking(AIC->GetPawn<APawn>(), avoidanceLocation, EDirection::Left);
		if (!tempLocation.IsZero()) {
			AIC->BBComponent->SetValueAsVector(TEXT("AvoidanceLocation"), tempLocation);
		}
		else {
			tempLocation = FindNoBlocking(AIC->GetPawn<APawn>(), avoidanceLocation, EDirection::Right);
		}

		if (!tempLocation.IsZero()) {
			AIC->BBComponent->SetValueAsVector(TEXT("AvoidanceLocation"), tempLocation);
		}
		else {
			AActor* pawn = AIC->GetPawn();
			AIC->BBComponent->SetValueAsVector(TEXT("TargetLocation"), pawn->GetActorLocation());
		}
	}

	return EBTNodeResult::Succeeded;
}

FVector UBTTask_SetAvoidanceLocation::FindNoBlocking(APawn* Pawn, FVector AvoidanceLocation, EDirection SearchDir)
{
	// Tracing 준비
	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	TArray<AActor*> actorToIgnore;
	actorToIgnore.Add(Pawn);

	// TargetLocation 방향 각도 계산
	FVector origin;
	FVector boxExtent;
	Pawn->GetActorBounds(false, origin, boxExtent);

	FVector pawnLocation = Pawn->GetActorLocation();
	FVector dir = FVector(AvoidanceLocation.X, AvoidanceLocation.Y, 0) - pawnLocation;
	FVector goalDir = dir.GetSafeNormal();
	float dot = FVector::DotProduct(Pawn->GetActorForwardVector(), goalDir);
	float AcosAngle = FMath::Acos(dot);
	float angle = FMath::RadiansToDegrees(AcosAngle);
	FVector cross = FVector::CrossProduct(FVector::ForwardVector, goalDir);
	float turnAngle;
	if (cross.Z > 0)
	{
		turnAngle = angle; //right
	}
	else if (cross.Z < 0)
	{
		turnAngle = -angle; //left
	}

	int turnDir;
	FVector traceStart;
	switch (SearchDir)
	{
	case EDirection::Center:
		turnDir = 2;
		traceStart = pawnLocation;
		break;
	case EDirection::Left:
		turnDir = 1;
		traceStart = pawnLocation + (-boxExtent.Y * 0.5f * Pawn->GetActorForwardVector());
		break;
	case EDirection::Right:
		turnDir = -1;
		traceStart = pawnLocation + (boxExtent.Y * 0.5f * Pawn->GetActorForwardVector());
		break;
	default:
		break;
	}
	float rotateUnit = 180 / (CountOfTracePerHalfround - 1);

	// 충돌체 반복 탐색
	for (int i = 0; i < CountOfTracePerHalfround; i++) {
		float addAngle = turnDir * rotateUnit * i;
		Pawn->SetActorRelativeRotation(FRotator(0, turnAngle, 0));
		Pawn->AddActorWorldRotation(FRotator(0, addAngle, 0));
		

		/*FHitResult outHitTemp;
		bool ResultTemp = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			pawnLocation,
			traceStart,
			objects,
			true,
			actorToIgnore,
			EDrawDebugTrace::ForDuration,
			outHitTemp,
			true,
			FLinearColor::Blue,
			FLinearColor::Green,
			1.0f
		);*/

		float dist = FVector().Dist(traceStart, AvoidanceLocation);
		FVector traceEnd = traceStart + (-Pawn->GetActorRightVector() * dist);
		FHitResult outHit;
		bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			traceStart,
			traceEnd,
			objects,
			true,
			actorToIgnore,
			EDrawDebugTrace::ForDuration,
			outHit,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			1.0f
		);

		if (!outHit.GetActor()) {
			return traceEnd;
		}
	}
	return FVector().ZeroVector;
}
