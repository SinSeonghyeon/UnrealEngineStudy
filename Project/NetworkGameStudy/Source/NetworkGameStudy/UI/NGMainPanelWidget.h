// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NGStatWidgetBase.h"
#include "NGMainPanelWidget.generated.h"

class UProgressBar;

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

protected:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthProgressBar;
	
};
