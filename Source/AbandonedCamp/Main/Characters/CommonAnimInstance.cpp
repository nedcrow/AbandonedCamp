// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonAnimInstance.h"
#include "CommonCharacter.h"

void UCommonAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UCommonAnimInstance::SetAttackAnimIndex()
{
#if WITH_EDITOR
#else
	if (GetOwningActor()) {
		ACommonCharacter* character = Cast<ACommonCharacter>(GetOwningActor());
		if (character && character->CountAttackAnim > 1) {
			AttackAnimIndex = FMath::RandRange(0, character->CountAttackAnim - 1);
		}
	}
#endif // WITH_EDITOR	
}
