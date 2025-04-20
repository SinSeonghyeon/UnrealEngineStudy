// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "../NGCharacterBase.h"
#include "NGPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UNGAnimInstance;
struct FInputActionValue;

UCLASS(config=Game)
class ANGPlayerCharacter : public ANGCharacterBase
{
	GENERATED_BODY()
public:
	ANGPlayerCharacter();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(Server, Reliable)
	void ServerRPCChangeWeaponMesh(FName WeaponName);

	UFUNCTION(Server, Reliable)
	void ServerRPCSpawnPalSphere();

	UFUNCTION(Server, Reliable)
	void ServerRPCSetPlayBuilindg(bool bEnable);

	// ---------------- EnhancedInput 관련 함수 ---------------------------
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// 공격키 입력시 호출되는 함수입니다.
	void AttackStarted(const FInputActionValue& Value);
	void AttackCompleted(const FInputActionValue& Value);
	void AttackTriggered(const FInputActionValue& Value);

	// 우클릭시 눌림시 함수입니다.
	void LockOn(const FInputActionValue& Value);

	// 우클릭 릴리즈시 호출되는 함수입니다.
	void LockOnCancel(const FInputActionValue& Value);

	void ThrowPalSphere();
	// ---------------- EnhancedInput 관련 함수 ---------------------------
protected:
	// To add mapping context
	virtual void BeginPlay();

	virtual void PostInitializeComponents() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void InitializeStatComponent() override;

	// 서버에서 호출됩니다.
	virtual void DieCharacter() override;
private:
	// Controller 기준으로 캐릭터 Yaw값을 회전시키는 포커스 요청을 기록하는 스택입니다.
	// 스택이 비어있다면 무브먼트 기준으로 변경됩니다.
	UPROPERTY(ReplicatedUsing = OnRep_ChangedFocusStack)
	TArray<FName> TargetFocusStack_Rep;

	UPROPERTY(ReplicatedUsing = OnRep_ChangedWeaponMesh)
	FName CurrentWeaponMeshName;

	// 팰스피어 투척을 위한 락온 여부를 확인하는 변수입니다.
	UPROPERTY(ReplicatedUsing = OnRep_ChangedIsPalSphereLockOn)
	bool bIsPalSphereLockOn = false;

private:
	UFUNCTION()
	void OnRep_ChangedFocusStack();

	UFUNCTION()
	void OnRep_ChangedWeaponMesh();

	UFUNCTION()
	void OnRep_ChangedIsPalSphereLockOn();

	// 입력 변수가 True라면 공격을 할 때 무브먼트 이동 기준이 아닌 컨트롤러 기준으로 변경합니다. false라면 반대 동작을 수행합니다.
	UFUNCTION(Server, Reliable)
	void ServerRPCTryFocusTarget(bool InUseControllerRotationYaw, FName InDebugName);

	UFUNCTION(Server, Reliable)
	void ServerRPCSetIsPalSphereLockOn(bool bLockOn);

	UFUNCTION()
	void OnRep_PlayBuilding();

private:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	// 무기 스켈레탈 메시 컴포넌트
	UPROPERTY(Transient)
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComponent;

	// 무기 스켈레탈 메시
	UPROPERTY(Transient)
	TObjectPtr<USkeletalMesh> WeaponMesh;

	// 펠스피어 메시
	UPROPERTY(Transient)
	TObjectPtr<USkeletalMesh> PalSphereMesh;

	UPROPERTY(ReplicatedUsing = OnRep_PlayBuilding)
	bool bIsPlayBuilding = false;
};

