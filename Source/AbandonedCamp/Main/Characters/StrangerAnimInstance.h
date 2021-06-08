// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonAnimInstance.h"
#include "StrangerCharacter.h"
#include "StrangerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ABANDONEDCAMP_API UStrangerAnimInstance : public UCommonAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
