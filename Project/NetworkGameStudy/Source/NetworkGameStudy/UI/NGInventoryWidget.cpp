// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NGInventoryWidget.h"
#include "NGInventorySlot.h"
#include "Components/WrapBox.h"


UNGInventoryWidget::UNGInventoryWidget(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{

}


void UNGInventoryWidget::NativeConstruct()
{
	TArray<UWidget*> AllChildren = SlotWrapBox->GetAllChildren();

	for (int i = 0; i < AllChildren.Num(); ++i)
	{
		if (UNGInventorySlot* InventorySlot = Cast<UNGInventorySlot>(AllChildren[i]))
		{
			InventorySlots.Add(InventorySlot);
			InventorySlot->SetItme(TEXT("None"), 0);
		}
	}
}

void UNGInventoryWidget::UpdateItemWidget(const TMap<FName, int32>& ItemList)
{
	for (int i = 0; i < InventorySlots.Num(); i++)
	{
		InventorySlots[i]->SetItme(TEXT("None"), 0);
	}

	int i = 0;

	for (const auto& Item : ItemList)
	{
		if (i >= InventorySlots.Num()) break;

		InventorySlots[i]->SetItme(Item.Key, Item.Value);

		++i;
	}
}
