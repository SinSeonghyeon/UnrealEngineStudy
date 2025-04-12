// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NGPalController.generated.h"

class UBehaviorTree;
struct FAIStimulus;

/**
 * 
 */
UCLASS()
class NETWORKGAMESTUDY_API ANGPalController : public AAIController
{
	GENERATED_BODY()
public:
	ANGPalController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	void StartEnemyTimer();
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> BehaviorTree_OwnedPal;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> BehaviorTree_UnOwnedPal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	FTimerHandle EnemyTimer;

	float LineOfSightTimer = 2.0f;
};
