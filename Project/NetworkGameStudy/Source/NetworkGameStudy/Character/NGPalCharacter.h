// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NGCharacterBase.h"
#include "NGPalCharacter.generated.h"

class ANGPalController;
class UWidgetComponent;

UCLASS(config=Game)
class ANGPalCharacter : public ANGCharacterBase
{
	GENERATED_BODY()
public:
	ANGPalCharacter();

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void InitializeStatComponent() override;

    UFUNCTION(BlueprintCallable, Category = "NGCustom")
    void UpdateMaxWalkSpeed(float NewSpeed);

	TObjectPtr<ANGCharacterBase> GetPalOwner() const { return PalOwner; };

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCTryCapture();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCCancelCapture();

	void SuccessCapture();

	// 펠스피어에 들어가는 애니메이션 시작
	void StartCapture();
	// 팰스피어에서 탈출하는 애니메이션 재생
	void StartEscape();
private:
	// 펠스피어에 들어가는 애니메이션 재생
	void PlayCapture();
	// 팰스피어에서 탈출하는 애니메이션 재생
	void PlayEscape();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> HeadUpWidgetComponent;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UMaterialInterface>> CachedMaterials;

	TObjectPtr<ANGPalController> CachedPalController;

	TObjectPtr<ANGCharacterBase> PalOwner;

	float InitTime = 0.4f;
	float CaptureTimer;
	float EscapeTimer;

	FTimerHandle CaptureHandle;
	FTimerHandle EscapeHandle;

	const float DeltaTime = 0.016f;

	FVector OriginMeshScale;
};

