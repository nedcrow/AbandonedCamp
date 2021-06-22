// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AttackEnd.h"
#include "../Characters/CommonAnimInstance.h"
#include "../Characters/CommonCharacter.h"
#include "../AI/CommonAIController.h"

void UAnimNotify_AttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	// 사용 애니메이션 재설정
	UCommonAnimInstance* animInstance = Cast<UCommonAnimInstance>(MeshComp->GetAnimInstance());
	if(animInstance) animInstance->SetAttackAnimIndex();

	// 적이 죽으면 Idle 상태로 전환
	ACommonCharacter* character = Cast<ACommonCharacter>(animInstance->GetOwningActor());
	if (character) {
		ACommonAIController* AIC = Cast<ACommonAIController>(character->GetController());
		ACommonCharacter* targetCharacter = Cast<ACommonCharacter>(AIC->CurrentEnermy);
		if (targetCharacter && targetCharacter->CurrentState == ECharacterState::Dead) {
			character->SetCurrentState(ECharacterState::Idle);
		}
	}
}