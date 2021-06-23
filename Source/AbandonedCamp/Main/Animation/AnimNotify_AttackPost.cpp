// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AttackPost.h"
#include "../Characters/CommonAnimInstance.h"
#include "../Characters/CommonCharacter.h"
#include "../AI/CommonAIController.h"
#include "Kismet/KismetMathLibrary.h"

void UAnimNotify_AttackPost::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	Super::Notify(MeshComp, Animation);

	UCommonAnimInstance* animInstance = Cast<UCommonAnimInstance>(MeshComp->GetAnimInstance());
	ACommonCharacter* character = Cast<ACommonCharacter>(animInstance->GetOwningActor());
	if (character) {

		// 타겟 바라보기
		ACommonAIController* AIC = Cast<ACommonAIController>(character->GetController());
		if (AIC && AIC->CurrentEnermy) {
			FRotator turnRotator = UKismetMathLibrary::FindLookAtRotation(character->GetActorLocation(), AIC->CurrentEnermy->GetActorLocation());
			character->SetActorRotation(turnRotator);
		}
	}
}
