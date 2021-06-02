// Fill out your copyright notice in the Description page of Project Settings.


#include "OneHandWeaponAnimInstance.h"
#include "CamperCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UOneHandWeaponAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	//Super::NativeUpdateAnimation(DeltaSeconds);
	ACamperCharacter* camper = Cast<ACamperCharacter>(TryGetPawnOwner());
	if (camper) {
		CurrentState = camper->CurrentState;
		MoveSpeed = camper->GetCharacterMovement()->Velocity.Size();
	}
}