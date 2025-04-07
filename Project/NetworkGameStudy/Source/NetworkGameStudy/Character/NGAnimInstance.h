// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NGAnimInstance.generated.h"

class UCharacterAnimData;

/**
 *
 */
UCLASS()
class NETWORKGAMESTUDY_API UNGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// 공격 애님 몽타주를 실행합니다.
	void PlayAttackMontage();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montages")
	UAnimMontage* AttackMontage;

};
