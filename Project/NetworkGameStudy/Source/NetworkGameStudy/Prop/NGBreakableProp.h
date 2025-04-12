// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NGBreakableProp.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class NETWORKGAMESTUDY_API ANGBreakableProp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANGBreakableProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


private:
	void BreakProp();

	void ShakeProp();

	void DestroyActor();

	void SpawnDropItem();

	// 메쉬가 파괴되는 연출을 호출합니다.
	UFUNCTION(NetMulticast, UnReliable)
	void MulticastRPCDestroyMesh();

	UFUNCTION(NetMulticast, UnReliable)
	void MulticastRPCShakeProp();



protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prop, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PropMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prop, meta = (AllowPrivateAccess = "true"))
	int32 CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prop, meta = (AllowPrivateAccess = "true"))
	TArray<FName> DropItmeIDs;
		
	UPROPERTY(EditAnywhere)
	UGeometryCollectionComponent* DestructibleMesh;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prop, meta = (AllowPrivateAccess = "true"))
	float ShakeTimeRemaining = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prop, meta = (AllowPrivateAccess = "true"))
	float ShakeSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prop, meta = (AllowPrivateAccess = "true"))
	float ShakeStrength = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prop, meta = (AllowPrivateAccess = "true"))
	float SpawnRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prop, meta = (AllowPrivateAccess = "true"))
	float SpawnHeightMin = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Prop, meta = (AllowPrivateAccess = "true"))
	float SpawnHeightMax = 200.0f;

	float CurrentShakeTimeRemaining = 3.0f;

	FRotator OriginalRotation;

	FTimerHandle ShakeHandle;

	FTimerHandle DestroyTimerHandle;
};
