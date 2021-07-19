// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MistPlane.generated.h"

UCLASS()
class ABANDONEDCAMP_API AMistPlane : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMistPlane();

protected:
	virtual void PostInitializeComponents() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	class UTextureRenderTarget2D* RenderTarget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DOT")
	float SecondPerOneHit = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DOT")
	float AttackPoint = 15.f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "DOT")
	float CurrentTime;

	/*
	* ��ں��� ���ų� �ǰ� ����� ��ںҰ� �ʹ� �ָ� �������� �������� �Դ´�.
	* �ǰݴ��: Camper
	*/
	void DamageOverTime();
};
