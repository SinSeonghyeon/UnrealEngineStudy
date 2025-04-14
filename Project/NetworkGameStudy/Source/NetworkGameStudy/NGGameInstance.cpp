// Fill out your copyright notice in the Description page of Project Settings.


#include "NGGameInstance.h"
#include "Manager/ItemDataTableManager.h"


UNGGameInstance::UNGGameInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	ItemDataTableManager = CreateDefaultSubobject<UItemDataTableManager>(TEXT("ItemDataTableManager"));

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Script/Engine.Material'/Game/00_Game/Character/Mesh/PalEmissiveMaterial.PalEmissiveMaterial'"));

	if (MaterialAsset.Succeeded())
	{
		EmissiveMaterial = MaterialAsset.Object;
	}
}

UItemDataTableManager* UNGGameInstance::GetItemDataTableManager(TObjectPtr<UWorld> InWorld)
{
	if (!InWorld) return nullptr;

	UNGGameInstance* GI = static_cast<UNGGameInstance*>(InWorld->GetGameInstance());
	return GI->ItemDataTableManager;
}

UMaterial* UNGGameInstance::GetEmissiveMaterial(TObjectPtr<UWorld> InWorld)
{
	if (!InWorld) return nullptr;

	UNGGameInstance* GI = static_cast<UNGGameInstance*>(InWorld->GetGameInstance());
	return GI->EmissiveMaterial;
}
