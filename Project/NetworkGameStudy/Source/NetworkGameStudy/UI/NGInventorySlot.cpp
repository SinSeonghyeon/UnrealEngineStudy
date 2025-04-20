// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NGInventorySlot.h"

void UNGInventorySlot::SetItme(FName InItemId, int32 Count)
{
	const FItemData* ItemData = UNGGameInstance::GetItemDataTableManager(GetWorld())->GetItemData(InItemId);

	if (ItemData)
	{
		ItemImage->SetBrushFromTexture(ItemData->Icon);

		FText CountFText = FText::FromString(FString::FromInt(Count));
		CountText->SetText(CountFText);

		ItemImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		CountText->SetVisibility(ESlateVisibility::Collapsed);
		ItemImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}
