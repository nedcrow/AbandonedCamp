// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CamperCharacter.h"
#include "OneHandWeaponAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API UOneHandWeaponAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	ECharacterState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MoveSpeed;

};
