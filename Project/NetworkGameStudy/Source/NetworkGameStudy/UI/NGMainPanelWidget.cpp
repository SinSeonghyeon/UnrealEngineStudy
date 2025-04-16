// Fill out your copyright notice in the Description page of Project Settings.


#include "NGMainPanelWidget.h"
#include "Components/ProgressBar.h"

void UNGMainPanelWidget::UpdateHPProgress(float Percent)
{
	HealthProgressBar->SetPercent(Percent);
}

void UNGMainPanelWidget::UpdateMaxHP(int32 HP)
{

}

void UNGMainPanelWidget::UpdateCurrentHP(int32 HP)
{

}