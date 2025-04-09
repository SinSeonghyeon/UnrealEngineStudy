// Fill out your copyright notice in the Description page of Project Settings.


#include "NGGameInstance.h"
#include "Manager/ItemDataTableManager.h"


UNGGameInstance::UNGGameInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	ItemDataTableManager = CreateDefaultSubobject<UItemDataTableManager>(TEXT("ItemDataTableManager"));
}

UItemDataTableManager* UNGGameInstance::GetItemDataTableManager(UObject* Object)
{
	if (!Object) return nullptr;

	UNGGameInstance* GI = static_cast<UNGGameInstance*>(Object->GetWorld()->GetGameInstance());
	return GI->ItemDataTableManager;
}
