// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AttackEnd.h"
#include "../Characters/CommonAnimInstance.h"
#include "../Characters/CommonCharacter.h"

void UAnimNotify_AttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UCommonAnimInstance* animInstance = Cast<UCommonAnimInstance>(MeshComp->GetAnimInstance());
	animInstance->SetAttackAnimIndex();

	ACommonCharacter* character = Cast<ACommonCharacter>(animInstance->GetOwningActor());
	character->bCanAttack = true;
}