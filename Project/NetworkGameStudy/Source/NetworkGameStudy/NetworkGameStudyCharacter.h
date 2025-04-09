// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "NetworkGameStudyCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UNGAnimInstance;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ANetworkGameStudyCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	ANetworkGameStudyCharacter();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	UNGAnimInstance* GetCachedAnimInstance() const { return CachedAnimInstance; }

protected:

	// ---------------- EnhancedInput 관련 함수 ---------------------------
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	// 공격키 입력시 호출되는 함수입니다.
	void Attack(const FInputActionValue& Value);

	void ResetAttack();

	// -------------- RPC --------------------
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCAttack_PlayAnim(float AttackStartTime);

	UFUNCTION(Client, Reliable)
	void ClientRPCPlayAttack_PlayAnim(ANetworkGameStudyCharacter* CharacterToPlay);

public:
	UFUNCTION(Server, Reliable)
	void ServerRPCAttack_HitCheck(FVector StartPos, float Radius);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

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

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	// 무기 스켈레탈 메시 컴포넌트
	UPROPERTY(Transient)
	USkeletalMeshComponent* WeaponMeshComponent;

	// 무기 스켈레탈 메시
	UPROPERTY(Transient)
	USkeletalMesh* WeaponMesh;

	UNGAnimInstance* CachedAnimInstance;
};

