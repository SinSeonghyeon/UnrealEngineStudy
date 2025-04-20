// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NGInventoryWidget.generated.h"

class UWrapBox;
class UNGInventorySlot;

/**
 * 
 */
UCLASS()
class NETWORKGAMESTUDY_API UNGInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UNGInventoryWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateItemWidget(const TMap<FName, int32>& ItemList);
	
	TArray<UNGInventorySlot*> InventorySlots; 
private:
	UPROPERTY(meta = (BindWidget))
	UWrapBox* SlotWrapBox;
};
