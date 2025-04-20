// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/NGInputWidgetBase.h"
#include "NGInteractionWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class NETWORKGAMESTUDY_API UNGInteractionWidget : public UNGInputWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetInteractionWidget(ANGInteractionActorBase* InInteractionActor);
	virtual bool InputKey() override;
	virtual bool InputKeyRelease() override;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractionText;
	
	ANGInteractionActorBase* InteractionActor;
};
