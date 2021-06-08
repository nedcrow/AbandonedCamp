// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AttackEnd.h"
#include "../Characters/CommonAnimInstance.h"

void UAnimNotify_AttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UCommonAnimInstance* AnimInstance = Cast<UCommonAnimInstance>(MeshComp->GetAnimInstance());
	AnimInstance->SetAttackAnimIndex();
}