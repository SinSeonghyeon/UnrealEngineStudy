// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Manager/ItemDataTableManager.h"
#include "NGGameInstance.generated.h"


/**
 *  게임 매니저 같은 역할을 할 게임 인스턴스입니다.
 * 게임 실행후 항상 사라지지 않으니..
 */
UCLASS()
class NETWORKGAMESTUDY_API UNGGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UNGGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static UItemDataTableManager* GetItemDataTableManager(UObject* Object);

private:
	UPROPERTY(Transient)
	UItemDataTableManager* ItemDataTableManager;
};
