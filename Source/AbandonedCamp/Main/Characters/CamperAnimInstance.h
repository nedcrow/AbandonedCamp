// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CamperCharacter.h"
#include "CommonAnimInstance.h"
#include "CamperAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API UCamperAnimInstance : public UCommonAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
