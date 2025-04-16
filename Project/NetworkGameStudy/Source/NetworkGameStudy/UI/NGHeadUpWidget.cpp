// Fill out your copyright notice in the Description page of Project Settings.


#include "NGHeadUpWidget.h"
#include "Components/ProgressBar.h"

void UNGHeadUpWidget::UpdateHPProgress(float Percent)
{
	HealthProgressBar->SetPercent(Percent);
}
