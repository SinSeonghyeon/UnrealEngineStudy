// Fill out your copyright notice in the Description page of Project Settings.


#include "NGMainPanelWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UNGMainPanelWidget::UpdateHPProgress(float Percent)
{
	HealthProgressBar->SetPercent(Percent);
}

void UNGMainPanelWidget::UpdateMaxHP(int32 HP)
{
	FText HPText = FText::FromString(FString::Printf(TEXT("%d"), HP));
	MaxHPTextBlock->SetText(HPText);
}

void UNGMainPanelWidget::UpdateCurrentHP(int32 HP)
{
	FText HPText = FText::FromString(FString::Printf(TEXT("%d"), HP));
	CurrentHPTextBlock->SetText(HPText);
}

void UNGMainPanelWidget::ToggleBuildingContextWidget()
{
	if (BuildingContextWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		BuildingContextWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		BuildingContextWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
