// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NGWeaponBase.h"
#include "NGWeaponPalSphere.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

/**
 *
 */
UCLASS()
class NETWORKGAMESTUDY_API ANGWeaponPalSphere : public ANGWeaponBase
{
	GENERATED_BODY()
public:
	ANGWeaponPalSphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Hit 이벤트 핸들러
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void DestroyPalSphere();

	// 팰 스피어를 흔드는 연출을 시작합니다.
	UFUNCTION(NetMultiCast, Reliable)
	void MultiCastRPCStartCaptureSequence(ANGPalCharacter* InTargetPal);

	// 팰 스피어를 흔드는 연출을 재생합니다.
	void PlayCaptureShake();
	
	// 포획 성공 여부 결정하고 상황에 맞는 이벤트를 출력.
	UFUNCTION(Server, Reliable)
	void ServerRPCEvaluateCaptureResult();
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> CollisionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	FTimerHandle DestroyTimeHandle;
	FTimerHandle CaptureShakeHandle;

	// 흔들림 관련 변수.
	float ShakeSpeed = 3000.0f;
	float ShakeStrength = 20.0f;

	float InitialTime = 0.15f;
	float CurrentInitialTimer = 0.2f;

	float ShakeTimeRemaining = 0.5f;
	float CurrentShakeTimeRemaining;
	float CurrentWaitTime;
	// 포획 중 상태 플래그
	bool bIsCapturing = false;
	bool bCaptureSuccessed = false;
	// 충돌 후 팰 스피어가 도착할 위치.
	FVector InitialLocation; 
	FVector DestinationLocation; 
	
	FRotator OriginalRotation;

	// 몇번이나 흔들릴지..
	int32 MaxShakeCount = 0;
	int32 CurrentShakeCount = 0;

	TObjectPtr<ANGPalCharacter> TargetPal;
};
