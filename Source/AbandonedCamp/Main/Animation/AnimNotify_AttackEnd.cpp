// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AttackEnd.h"
#include "../Characters/CommonAnimInstance.h"

void UAnimNotify_AttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

//#if WITH_EDITOR
//#else
	UCommonAnimInstance* animInstance = Cast<UCommonAnimInstance>(MeshComp->GetAnimInstance());
	if(animInstance) animInstance->SetAttackAnimIndex();
//#endif // WITH_EDITOR & else
}