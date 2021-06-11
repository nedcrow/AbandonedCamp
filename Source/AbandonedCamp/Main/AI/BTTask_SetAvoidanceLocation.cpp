// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetAvoidanceLocation.h"
#include "CommonAIController.h"

#include "BehaviorTree/BlackBoardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Landscape.h"

EBTNodeResult::Type UBTTask_SetAvoidanceLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACommonAIController* AIC = Cast<ACommonAIController>(OwnerComp.GetAIOwner());
	FVector targetLocation = AIC->BBComponent->GetValueAsVector(TEXT("TargetLocation"));
	//FVector avoidanceLocation = AIC->BBComponent->GetValueAsVector(TEXT("TargetLocation"));


	TArray<FVector> LeftLocations;
	TArray<FVector> RightLocations;
	if (AIC && AIC->IsLocalController()) {
		APawn* pawn = AIC->GetPawn<APawn>();
		if (pawn) {
			LeftLocations.Add(pawn->GetActorLocation());
			RightLocations.Add(pawn->GetActorLocation());

			AActor* targetActor = Cast<AActor>(AIC->BBComponent->GetValueAsObject(TEXT("TargetActor")));

			// 왼쪽 동선 추적
			FVector startLocation = pawn->GetActorLocation();
			for (int i = 0; i < 10; i++) {
				if(FVector::Distance(startLocation, targetLocation) < 100) {break;}
				startLocation = FindNoBlocking(pawn, startLocation, targetLocation, targetActor, EDirection::Left);
				LeftLocations.Add(startLocation);
			}

			// 오른쪽 동선 추적
			startLocation = pawn->GetActorLocation();
			for (int i = 0; i < 10; i++) {
				if (FVector::Distance(startLocation, targetLocation) < 100) { break; }
				startLocation = FindNoBlocking(pawn, startLocation, targetLocation, targetActor, EDirection::Right);
				RightLocations.Add(startLocation);
			}

			// 총 동선 거리 비교 후 짧은 쪽 선택
			/*float LeftDist = 0;
			float RightDist = 0;
			for (int i = 1; i < LeftLocations.Num(); i++) {
				LeftDist+= FVector::Distance(LeftLocations[i - 1], LeftLocations[i]);
			}
			for (int i = 1; i < LeftLocations.Num(); i++) {
				RightDist += FVector::Distance(LeftLocations[i - 1], LeftLocations[i]);
			}
			if (LeftDist <= RightDist) {
				if (LeftLocations.Num() > 2) {
					AIC->SetTargetLocation(LeftLocations[1]);
					if(AIC->BBComponent->GetValueAsEnum(TEXT("CurrentMoveState")) != (uint8)EMoveState::ToAvoidance){
						AIC->BBComponent->SetValueAsEnum(TEXT("CurrentMoveState"), (uint8)EMoveState::ToAvoidance);
						UE_LOG(LogTemp, Warning, TEXT("LeftLocations: %f, %f, %f / Dist: %f"), LeftLocations[1].X, LeftLocations[1].Y, LeftLocations[1].Z, LeftDist);
					}					
				}
				else {
					if (AIC->BBComponent->GetValueAsEnum(TEXT("CurrentMoveState")) != (uint8)EMoveState::ToTarget) {
						AIC->BBComponent->SetValueAsEnum(TEXT("CurrentMoveState"), (uint8)EMoveState::ToTarget);
					}
				}
			}
			else {
				if (RightLocations.Num() > 2) {
					AIC->SetTargetLocation(RightLocations[1]);
					if (AIC->BBComponent->GetValueAsEnum(TEXT("CurrentMoveState")) != (uint8)EMoveState::ToAvoidance) {
						AIC->BBComponent->SetValueAsEnum(TEXT("CurrentMoveState"), (uint8)EMoveState::ToAvoidance);
						UE_LOG(LogTemp, Warning, TEXT("RightLocations: %f, %f, %f / Dist: %f"), RightLocations[1].X, RightLocations[1].Y, RightLocations[1].Z, RightDist);
					}					
				}
				else {
					if (AIC->BBComponent->GetValueAsEnum(TEXT("CurrentMoveState")) != (uint8)EMoveState::ToTarget) {
						AIC->BBComponent->SetValueAsEnum(TEXT("CurrentMoveState"), (uint8)EMoveState::ToTarget);
					}
				}
			}*/

			// targetLocation 저장
			if (!startLocation.IsZero()) {
				AIC->BBComponent->SetValueAsVector(TEXT("AvoidanceLocation"), targetLocation);
			}
			else {
				AIC->SetTargetLocation(pawn->GetActorLocation());
				UE_LOG(LogTemp, Warning, TEXT("No path actor %s -- UBTTask_SetAvoidanceLocation"), *pawn->GetFName().ToString());
			}
		}		
	}

	return EBTNodeResult::Succeeded;
}

FVector UBTTask_SetAvoidanceLocation::FindNoBlocking(APawn* Pawn, FVector ALocation, FVector BLocation, AActor* TargetActor, EDirection SearchDir)
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
	//if(TargetActor)actorToIgnore.Add(TargetActor);

	// TargetLocation 방향 각도 계산	
	FRotator turnRotator = UKismetMathLibrary::FindLookAtRotation(ALocation, FVector(BLocation.X, BLocation.Y, 0));

	// 충돌체 반복 탐색
	float rotateUnit = 180 / (CountOfTracePerHalfround - 1);
	float lastDist=0;
	for (int i = 0; i < CountOfTracePerHalfround; i++) {
		// 트레이스 시작, 도착점 위치 계산
		FVector origin;
		FVector boxExtent;
		Pawn->GetActorBounds(false, origin, boxExtent);
		float addAngle = rotateUnit * i;
		float turnAngle;
		float dist;
		FVector sideTraceStart;
		FVector sideTraceEnd;
		FVector centerTraceStart;
		FVector centerTraceEnd;
		FVector result;
		switch (SearchDir)
		{
		case EDirection::Center:
			sideTraceStart = ALocation;
			break;
		case EDirection::Left:	
			turnAngle = turnRotator.Yaw + addAngle + 90;
			sideTraceStart = ALocation + (boxExtent.Y * 0.5f * FRotator(0.f, turnAngle, 0.f).Vector());
			dist = lastDist != 0 ? lastDist + 50 : FVector().Distance(sideTraceStart, BLocation);
			sideTraceEnd = sideTraceStart + (dist * FRotator(0.f, turnAngle - 90.0f, 0.f).Vector());
			result = sideTraceStart + (lastDist * FRotator(0.f, turnAngle - 90.0f, 0.f).Vector());
			break;
		case EDirection::Right:
			turnAngle = turnRotator.Yaw - addAngle - 90;
			sideTraceStart = ALocation + (boxExtent.Y * 0.5f * FRotator(0.f, turnAngle, 0.f).Vector());
			dist = lastDist != 0 ? lastDist + 50 : FVector().Distance(sideTraceStart, BLocation);
			sideTraceEnd = sideTraceStart + (dist * FRotator(0.f, turnAngle + 90.0f, 0.f).Vector());
			result = sideTraceStart + (lastDist * FRotator(0.f, turnAngle + 90.0f, 0.f).Vector());
			break;
		default:
			break;
		}

		// 디버깅 트레이스
		/*FHitResult outHitTemp;
		bool startPointCast = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			ALocation,
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
		);*/

		FHitResult outHit2;
		bool targetPointCast = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			ALocation,
			sideTraceEnd,
			objects,
			true,
			actorToIgnore,
			EDrawDebugTrace::ForDuration,
			outHit2,
			true,
			FLinearColor::Yellow,
			FLinearColor::Green,
			5.0f
		);

		FHitResult outHit;
		bool cast = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			sideTraceStart,
			sideTraceEnd,
			objects,
			true,
			actorToIgnore,
			EDrawDebugTrace::None,
			outHit,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			5.0f
		);

		if ((!outHit.GetActor() || outHit.GetActor() == TargetActor) && (!outHit2.GetActor() || outHit2.GetActor() == TargetActor)) {
			FHitResult outHitTest;
			bool castLog = UKismetSystemLibrary::LineTraceSingleForObjects(
				GetWorld(),
				ALocation,
				sideTraceEnd,
				objects,
				true,
				actorToIgnore,
				EDrawDebugTrace::ForDuration,
				outHitTest,
				true,
				FLinearColor::Red,
				FLinearColor::Green,
				5.0f
			);
			return lastDist == 0 ? sideTraceEnd : result;
		}
		else {
			lastDist = FVector::Distance(sideTraceStart, outHit.ImpactPoint);
		}
	}
	return FVector().ZeroVector;
}
