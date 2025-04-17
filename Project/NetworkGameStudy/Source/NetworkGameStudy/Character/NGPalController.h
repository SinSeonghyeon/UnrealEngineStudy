// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NGPalController.generated.h"

class UBehaviorTree;
struct FAIStimulus;
class UBehaviorTreeComponent;

DECLARE_MULTICAST_DELEGATE(FOnFindTarget);
DECLARE_MULTICAST_DELEGATE(FOnLostTarget);
/**
 * 
 */
UCLASS()
class NETWORKGAMESTUDY_API ANGPalController : public AAIController
{
	GENERATED_BODY()
public:
	ANGPalController();

public:
	void PauseBehaviorTree();

	void ResumeBehaviorTree();
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	void StartEnemyResetTargetTimer();

public:
	FOnFindTarget OnFindTarget;
	FOnLostTarget OnLostTarget;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> BehaviorTree_OwnedPal;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> BehaviorTree_UnOwnedPal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	TObjectPtr<UBehaviorTreeComponent> CachedBehaviorTreeComponent;

	TObjectPtr<ANGPalCharacter> MyPalCharacter;

	FTimerHandle EnemyTimer;

	float LineOfSightTimer = 2.0f;
};
