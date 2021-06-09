// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetAvoidanceLocation.h"
#include "CommonAIController.h"

#include "BehaviorTree/BlackBoardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Landscape.h"

EBTNodeResult::Type UBTTask_SetAvoidanceLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACommonAIController* AIC = Cast<ACommonAIController>(OwnerComp.GetAIOwner());
	FVector avoidanceLocation = AIC->BBComponent->GetValueAsVector(TEXT("TargetLocation"));
	//FVector avoidanceLocation = AIC->BBComponent->GetValueAsVector(TEXT("TargetLocation"));


	TArray<FVector> TempLocations;
	if (AIC && AIC->IsLocalController()) {
		APawn* pawn = AIC->GetPawn<APawn>();
		if (pawn) {
			TempLocations.Add(pawn->GetActorLocation());

			AActor* targetActor = Cast<AActor>(AIC->BBComponent->GetValueAsObject(TEXT("")));
			FVector tempLocation = FindNoBlocking(pawn, avoidanceLocation, targetActor, EDirection::Left);
			if (!tempLocation.IsZero()) {
				TempLocations.Add(tempLocation);
				//AIC->BBComponent->SetValueAsVector(TEXT("AvoidanceLocation"), tempLocation);
			}
			else {
				tempLocation = FindNoBlocking(pawn, avoidanceLocation, targetActor, EDirection::Right);
			}

			if (!tempLocation.IsZero()) {
				AIC->BBComponent->SetValueAsVector(TEXT("AvoidanceLocation"), tempLocation);
			}
			else {
				AIC->BBComponent->SetValueAsVector(TEXT("TargetLocation"), pawn->GetActorLocation());
			}
		}		
	}

	return EBTNodeResult::Succeeded;
}

FVector UBTTask_SetAvoidanceLocation::FindNoBlocking(APawn* Pawn, FVector AvoidanceLocation, AActor* TargetActor, EDirection SearchDir)
{
	// Tracing 충돌체 관리
	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	TArray<AActor*> actorToIgnore;
	ALandscape* land = Cast<ALandscape>(UGameplayStatics::GetActorOfClass(GetWorld(), ALandscape::StaticClass()));
	actorToIgnore.Add(Pawn);
	actorToIgnore.Add(land);
	if(TargetActor)actorToIgnore.Add(TargetActor);

	// TargetLocation 방향 각도 계산
	FVector pawnLocation = Pawn->GetActorLocation();
	FVector dir = FVector(AvoidanceLocation.X, AvoidanceLocation.Y, 0) - pawnLocation;
	FVector goalDir = dir.GetSafeNormal();
	float dot = FVector::DotProduct(Pawn->GetActorForwardVector(), goalDir);
	float AcosAngle = FMath::Acos(dot);
	float angle = FMath::RadiansToDegrees(AcosAngle);
	FVector cross = FVector::CrossProduct(FVector::ForwardVector, goalDir);
	
	// trace 시작점 계산
	FVector origin;
	FVector boxExtent;
	Pawn->GetActorBounds(false, origin, boxExtent);
	int turnDir=2;
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
	float rotateUnit = 180 / (CountOfTracePerHalfround-1);

	// 충돌체 반복 탐색
	for (int i = 0; i < CountOfTracePerHalfround; i++) {
		float addAngle = rotateUnit * i;
		float turnAngle = 0;
		if (cross.Z > 0)
		{
			turnAngle = (angle + addAngle); //right
			traceStart = pawnLocation + (boxExtent.Y * 0.5f * FRotator(0.f, turnAngle, 0.f).Vector());
		}
		else if (cross.Z < 0)
		{
			turnAngle = -(angle + addAngle); //left
			traceStart = pawnLocation + (-boxExtent.Y * 0.5f * FRotator(0.f, turnAngle, 0.f).Vector());
		}
		Pawn->SetActorRelativeRotation(FRotator(0, turnAngle, 0));

		FHitResult outHitTemp;
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
			5.0f
		);

		float dist = FVector().Dist(traceStart, AvoidanceLocation);

		FVector traceEnd = traceStart + (-dist * FRotator(0.f, turnAngle + 90.0f, 0.f).Vector());
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
			5.0f
		);

		if (!outHit.GetActor()) {
			return traceEnd;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("HitActor - %s"), *outHit.GetActor()->GetFName().ToString());
		}
	}
	return FVector().ZeroVector;
}
