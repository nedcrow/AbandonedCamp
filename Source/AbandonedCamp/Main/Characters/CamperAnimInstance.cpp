// Fill out your copyright notice in the Description page of Project Settings.


#include "CamperAnimInstance.h"
#include "CamperCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCamperAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);
	ACamperCharacter* camper = Cast<ACamperCharacter>(TryGetPawnOwner());
	if (camper) {
		// SetState
		CurrentState = camper->CurrentState;

		// Moving Speed
		MoveSpeed = camper->GetCharacterMovement()->Velocity.Size();
	}
}