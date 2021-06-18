// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AttackStart.h"
#include "../Characters/CommonAnimInstance.h"
#include "../Characters/CommonCharacter.h"

void UAnimNotify_AttackStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	Super::Notify(MeshComp, Animation);

	//#if WITH_EDITOR
	//#else
	UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_AttackStart - 1"));

	UCommonAnimInstance* animInstance = Cast<UCommonAnimInstance>(MeshComp->GetAnimInstance());
	ACommonCharacter* character = Cast<ACommonCharacter>(animInstance->GetOwningActor());
	if (character) {
		character->SetGenerateOverlapEventOfWeapons(true);
		character->bCanAttack = true;
		UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_AttackStart - 2"));
	}
	//#endif // WITH_EDITOR & else
}

