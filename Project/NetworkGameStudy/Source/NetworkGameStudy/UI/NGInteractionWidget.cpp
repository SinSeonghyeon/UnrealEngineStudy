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

	if (InteractionActor)
	{
		Cast<ANGPlayerController>(GetWorld()->GetFirstPlayerController())->ServerRPCRequestDoInteraction(InteractionActor);
	}

	return true;
}

bool UNGInteractionWidget::InputKeyRelease()
{
	Super::InputKeyRelease();

	if (InteractionActor)
	{
		Cast<ANGPlayerController>(GetWorld()->GetFirstPlayerController())->ServerRPCRequestCancelInteraction(InteractionActor);
	}
	
	return true;
}
