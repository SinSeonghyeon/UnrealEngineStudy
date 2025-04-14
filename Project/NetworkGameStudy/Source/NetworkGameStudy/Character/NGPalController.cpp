// Fill out your copyright notice in the Description page of Project Settings.

#include "NGPalController.h"
#include "NGPalCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIController.h"
#include "BehaviorTree\BlackboardComponent.h"
#include "../NetworkGameStudy.h"

ANGPalController::ANGPalController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ANGPalController::OnTargetPerceptionUpdated);
}

void ANGPalController::PauseBehaviorTree()
{
	FString Capture = TEXT("Capture");
	CachedBehaviorTreeComponent->PauseLogic(Capture);
}

void ANGPalController::ResumeBehaviorTree()
{
	FString Capture = TEXT("CaptureCancel");
	CachedBehaviorTreeComponent->ResumeLogic(Capture);
}


void ANGPalController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{	
	NG_LOG(LogTemp, Log, TEXT("EndPlay"));
	Super::EndPlay(EndPlayReason);
}

void ANGPalController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	TObjectPtr<ANGPalCharacter> PalCharacter = Cast<ANGPalCharacter>(InPawn);

	if (PalCharacter)
	{
		if (PalCharacter->GetPalOwner())
			RunBehaviorTree(BehaviorTree_OwnedPal);
		else
			RunBehaviorTree(BehaviorTree_UnOwnedPal);

		CachedBehaviorTreeComponent = GetComponentByClass<UBehaviorTreeComponent>();
	}

}

void ANGPalController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	//NG_LOG(LogTemp, Log, TEXT("OnTargetPerceptionUpdated Start"));

	FName Target = TEXT("Player");
	if (Actor->ActorHasTag(Target))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			//NG_LOG(LogTemp, Log, TEXT("FindTarget"));
			GetWorldTimerManager().ClearTimer(EnemyTimer);

			Blackboard->SetValueAsBool(TEXT("HasLineOfSight"), true);
			Blackboard->SetValueAsObject(TEXT("TargetActor"), Actor);
		}
		else
		{
			//NG_LOG(LogTemp, Log, TEXT("LostTarget"));
			GetWorldTimerManager().SetTimer(EnemyTimer, this, &ANGPalController::StartEnemyTimer, LineOfSightTimer, false);
		}
	}
	else
	{
		//NG_LOG(LogTemp, Log, TEXT("Nothing"));
	}
}

void ANGPalController::StartEnemyTimer()
{
	Blackboard->SetValueAsBool(TEXT("HasLineOfSight"), false);
	Blackboard->SetValueAsObject(TEXT("TargetActor"), nullptr);
}
