// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "NGCharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UNGAnimInstance;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ANGCharacterBase : public ACharacter
{
	GENERATED_BODY()
public:
	ANGCharacterBase();

public:
	UNGAnimInstance* GetCachedAnimInstance() const { return CachedAnimInstance; }

	// 어택함수 구현부입니다. 서버에서도 사용하기 위해 분리하였습니다.
	void Attack_Implementation();

protected:
	void ResetAttack();

	// -------------- RPC --------------------
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCAttack_PlayAnim(float AttackStartTime);

	UFUNCTION(Client, Reliable)
	void ClientRPCPlayAttack_PlayAnim(ANGCharacterBase* CharacterToPlay);

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayAnim(FName AnimName);

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPCPlay_PlayAnim(FName AnimName);

	// 이 함수는 팰이 공격할 때 호출되는 함수입니다. 팰의 경우 서버에서 AI가 관리되고 있으니 공격 시간을 기록할 필요는 없을 것 같습니다.
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPCPlayAttack_PlayAnim();

	UFUNCTION(NetMulticast, UnReliable)
	void MultiCastRPCPlayHit_PlayAnim();
public:
	UFUNCTION(Server, Reliable)
	void ServerRPCAttack_HitCheck(FVector StartPos, float Radius);

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	 virtual void PostInitializeComponents() override;

protected:
	// -------------------- 공격과 관련된 변수들 ----------------------
	// 현재 공격이 가능한지를 저장하는 변수
	bool bCanAttack = true;

	// 마지막 공격을 한시간이 언제인지 기록합니다. 너무 빠르게 공격하는 것을 막기 위함.
	float LastAttackStartTime = 0;

	// 공격은 1초에 한번 가능합니다.
	float AttackTime = 1.0f;

	FTimerHandle AttackTimerHandle;

private:
	TObjectPtr<UNGAnimInstance> CachedAnimInstance;
};

