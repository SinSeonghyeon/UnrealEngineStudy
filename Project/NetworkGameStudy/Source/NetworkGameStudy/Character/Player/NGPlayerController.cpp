// Fill out your copyright notice in the Description page of Project Settings.


#include "NGPlayerController.h"
#include "../../SubSystem/UIManagerWorldSubsystem.h"

void ANGPlayerController::BeginPlay()
{
	if (GetNetMode() == ENetMode::NM_Client)
	{
		if (UUIManagerWorldSubsystem* UIManager = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>())
		{
			UIManager->ShowMainUI(this);
		}
	}
}
