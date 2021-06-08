// Fill out your copyright notice in the Description page of Project Settings.


#include "StrangerAnimInstance.h"
#include "StrangerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UStrangerAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);
	AStrangerCharacter* stranger = Cast<AStrangerCharacter>(TryGetPawnOwner());
	if (stranger) {
		// SetState
		CurrentState = stranger->CurrentState;

		// SetMovingSpeed
		float footSpeed;
		GetCurveValue(TEXT("FootSpeed"), footSpeed);		
		stranger->GetCharacterMovement()->MaxWalkSpeed = stranger->WalkSpeed * footSpeed;
		MoveSpeed = stranger->GetCharacterMovement()->Velocity.Size();
	}
}