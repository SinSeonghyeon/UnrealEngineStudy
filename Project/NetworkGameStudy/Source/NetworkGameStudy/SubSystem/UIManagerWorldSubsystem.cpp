// Fill out your copyright notice in the Description page of Project Settings.

#include "UIManagerWorldSubsystem.h"
#include "Blueprint/UserWidget.h"


UUIManagerWorldSubsystem::UUIManagerWorldSubsystem()
{
	// 클래스 로드
	static ConstructorHelpers::FClassFinder<UNGStatWidgetBase> MainUIBPClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/00_Game/UI/BP_Main_Panel.BP_Main_Panel_C'"));
	if (MainUIBPClass.Succeeded())
	{
		MainUIClass = MainUIBPClass.Class;
	}
}

void UUIManagerWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UUIManagerWorldSubsystem::Deinitialize()
{
	HideMainUI();
	Super::Deinitialize();
}

void UUIManagerWorldSubsystem::ShowMainUI(APlayerController* PlayerController)
{
	if (!MainUIInstance && MainUIClass)
	{
		if (!PlayerController) return;

		MainUIInstance = CreateWidget<UNGStatWidgetBase>(PlayerController, MainUIClass);
		if (MainUIInstance)
		{
			MainUIInstance->AddToViewport();
		}
	}
}

void UUIManagerWorldSubsystem::HideMainUI()
{
	if (MainUIInstance)
	{
		MainUIInstance->RemoveFromParent();
		MainUIInstance = nullptr;
	}
}