// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataTableManager.generated.h"

struct FItemData;

/**
 *
 */
UCLASS()
class NETWORKGAMESTUDY_API UItemDataTableManager : public UObject
{
	GENERATED_BODY()
public:
	UItemDataTableManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	const FItemData* GetItemData(FName ItemID) const;

private:
	UPROPERTY(EditDefaultsOnly)
	UDataTable* ItemDataTable;
};
