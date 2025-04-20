// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NGStatWidgetBase.h"
#include "NGMainPanelWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UNGInventoryWidget;

/**
 *
 */
UCLASS()
class NETWORKGAMESTUDY_API UNGMainPanelWidget : public UNGStatWidgetBase
{
	GENERATED_BODY()

public:
	virtual void UpdateHPProgress(float Percent);
	virtual void UpdateMaxHP(int32 HP);
	virtual void UpdateCurrentHP(int32 HP);

	bool ToggleBuildingContextWidget();
	bool ToggleInventoryWidget();

	void SetInteractionWidget(ANGInteractionActorBase* InteractionActor);
	void UpdateItemWidget(const TMap<FName, int32>& ItemList);
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentHPTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxHPTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UNGBuildingContextWidget> BuildingContextWidget;	
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UNGInventoryWidget> InventoryWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UNGInteractionWidget> InteractionWidget;
};
