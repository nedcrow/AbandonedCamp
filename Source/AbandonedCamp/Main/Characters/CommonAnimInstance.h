// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CommonCharacter.h"
#include "CommonAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API UCommonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		ECharacterState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		int AttackAnimIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		int HitAnimIndex = 0;

	UFUNCTION()
		void SetAttackAnimIndex();
};
