// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

	// RowName을 ItemID로 사용.
    //UPROPERTY(EditAnywhere, BlueprintReadWrite)
    //FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class ANGDropItemActor> DropActorClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon;
};