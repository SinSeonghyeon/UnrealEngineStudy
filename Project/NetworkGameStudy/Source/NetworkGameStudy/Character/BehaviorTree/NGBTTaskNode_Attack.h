// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "NGBTTaskNode_Attack.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKGAMESTUDY_API UNGBTTaskNode_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	// 기본 생성자
	UNGBTTaskNode_Attack();

protected:
	// Task가 실행될 때 호출되는 함수
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
};
