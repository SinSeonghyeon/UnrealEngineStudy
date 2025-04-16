// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NGStatWidgetBase.h"
#include "NGHeadUpWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class NETWORKGAMESTUDY_API UNGHeadUpWidget : public UNGStatWidgetBase
{
	GENERATED_BODY()
public:
	virtual void UpdateHPProgress(float Percent) override;

protected:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthProgressBar;
	

};
