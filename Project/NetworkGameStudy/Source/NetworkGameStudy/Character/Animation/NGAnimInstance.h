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

	// 피격 애님 몽타주를 실행합니다.
	void PlayHitMontage();

	// 팰스피어를 던지는 준비자세를 취합니다.
	void PlayLockOn();

	// 팰스피어를 던집니다.
	void PlayThrow();
	
	// 팰스피어를 던지는 준비자세를 취소합니다.
	void StopLockOn();

	// 구조가 굉장히 별로지만.. 고치기엔 시간이 오래걸리니..
	// 추후에 고친다면 맵 자료구조를 이용하여 몽타주를 저장할 것 같습니다.
	void PlayAnimFromName(FName AnimName);

	void SetBuilding(bool bEnable);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montages")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montages")
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montages")
	TObjectPtr<UAnimMontage> OnLockMontage;

	UPROPERTY(BlueprintReadOnly, Category = "Custom")
	bool bIsBuilding = false;
};
