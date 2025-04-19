// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/NGInteractionWidget.h"
#include "Components/TextBlock.h"

void UNGInteractionWidget::SetInteractionWidget(ANGInteractionActorBase* InInteractionActor)
{
	InteractionText->SetText(InInteractionActor->GetInteractionText());
	InteractionActor = InInteractionActor;
}

bool UNGInteractionWidget::InputKey()
{
	Super::InputKey();

	if(InteractionActor)
		InteractionActor->DoIneraction();

	return true;
}
