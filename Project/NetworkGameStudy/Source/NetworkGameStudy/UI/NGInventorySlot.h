// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NGInventorySlot.generated.h"

class UTextBlock;
class UImage;


/**
 *
 */
UCLASS()
class NETWORKGAMESTUDY_API UNGInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetItme(FName InItemId, int32 Count);

	FName GetItemId() { return ItemId; }

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountText;

	UPROPERTY(meta = (BindWidget))
	UImage* BackgroundImage;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage;

	FName ItemId;
};
