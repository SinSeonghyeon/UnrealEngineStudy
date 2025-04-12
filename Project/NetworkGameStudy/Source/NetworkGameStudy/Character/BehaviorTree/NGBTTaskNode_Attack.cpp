// Fill out your copyright notice in the Description page of Project Settings.


#include "NGBTTaskNode_Attack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../NGCharacterBase.h"
#include "AIController.h"

UNGBTTaskNode_Attack::UNGBTTaskNode_Attack()
{

}

EBTNodeResult::Type UNGBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Actor를 가져오기 위한 Blackboard 사용 예시
	// UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (ANGCharacterBase* MyCharacter = Cast<ANGCharacterBase>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		MyCharacter->Attack_Implementation();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UNGBTTaskNode_Attack - Owner is not a valid character"));
	}

	// Task 실패로 처리
	return EBTNodeResult::Failed;
}
