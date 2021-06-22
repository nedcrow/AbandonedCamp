// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AttackPause.h"
#include "../Characters/CommonAnimInstance.h"
#include "../Characters/CommonCharacter.h"

void UAnimNotify_AttackPause::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	Super::Notify(MeshComp, Animation);

	UCommonAnimInstance* animInstance = Cast<UCommonAnimInstance>(MeshComp->GetAnimInstance());
	ACommonCharacter* character = Cast<ACommonCharacter>(animInstance->GetOwningActor());
	if (character) {
		character->SetGenerateOverlapEventOfWeapons(false);
		character->bCanAttack = false;
	}
}