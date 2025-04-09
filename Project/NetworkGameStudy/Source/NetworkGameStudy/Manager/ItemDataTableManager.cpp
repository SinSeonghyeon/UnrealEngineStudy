#include "ItemDataTableManager.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataTableManager.h"
#include "../Item/ItemData.h"


UItemDataTableManager::UItemDataTableManager(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemTableObj(
		TEXT("/Script/Engine.DataTable'/Game/Item/ItemDataTable.ItemDataTable'")
	);

	if (ItemTableObj.Succeeded())
	{
		ItemDataTable = ItemTableObj.Object;
		UE_LOG(LogTemp, Log, TEXT("DataTable loaded successfully."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load DataTable!"));
	}
}

const FItemData* UItemDataTableManager::GetItemData(FName ItemID) const
{
	static const FString Context = TEXT("Item Data Lookup");
	return ItemDataTable->FindRow<FItemData>(ItemID, Context);
}
