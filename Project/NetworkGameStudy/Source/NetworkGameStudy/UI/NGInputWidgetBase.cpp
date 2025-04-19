// Fill out your copyright notice in the Description page of Project Settings.


#include "NGInputWidgetBase.h"
#include "Components/PanelWidget.h"

//UNGInputWidgetBase::UNGInputWidgetBase(const FObjectInitializer& ObjectInitializer) :
//	Super(ObjectInitializer)
//{
//}

bool UNGInputWidgetBase::InputMouseClick()
{
	for (const auto& Widget : UserWidgetChildren)
	{
		if (Widget->InputMouseClick())
			return true;
	}

	return false;
}


bool UNGInputWidgetBase::InputMouseMove(const FInputActionValue& Value)
{
	for (const auto& Widget : UserWidgetChildren)
	{
		if (Widget->InputMouseMove(Value))
			return true;
	}

	return false;
}


bool UNGInputWidgetBase::InputKey()
{
	for (const auto& Widget : UserWidgetChildren)
	{
		if (Widget->GetVisibility() != ESlateVisibility::Collapsed && Widget->InputKey())
			return true;
	}

	return false;
}

void UNGInputWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	UObject* Outer = GetParent();
	while (Outer)
	{
		if (UNGInputWidgetBase* NGStatWidgetBase = Cast<UNGInputWidgetBase>(Outer))
		{
			SetInputWidgetBaseParent(NGStatWidgetBase);
			break;
		}
		Outer = Outer->GetOuter();
	}
}

void UNGInputWidgetBase::SetInputWidgetBaseParent(const TObjectPtr<UNGInputWidgetBase> Parent)
{
	Parent->SetInputWidgetBaseChild(this);
}

void UNGInputWidgetBase::SetInputWidgetBaseChild(const TObjectPtr<UNGInputWidgetBase> Child)
{
	UserWidgetChildren.Add(Child);
}
