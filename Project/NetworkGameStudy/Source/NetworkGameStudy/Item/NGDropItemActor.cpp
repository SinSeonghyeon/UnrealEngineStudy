// Fill out your copyright notice in the Description page of Project Settings.


#include "NGDropItemActor.h"
#include "../NGGameInstance.h"
#include "ItemData.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ANGDropItemActor::ANGDropItemActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MeshComponent->SetCollisionProfileName(TEXT("DropActor"));
	MeshComponent->SetSimulatePhysics(true);

	RootComponent = MeshComponent;

	bReplicates = true;
}

void ANGDropItemActor::Initialize(FName InItemID)
{
	ItemID = InItemID;

	const FItemData* ItemData = UNGGameInstance::GetItemDataTableManager(this)->GetItemData(ItemID);
	if (ItemData)
	{
		UE_LOG(LogTemp, Log, TEXT("Loaded item: %s"), *ItemData->ItemName.ToString());

		ReplicatedMesh = ItemData->StaticMesh;
		// 서버에도 메시를 반영하기 위함.
		// OnRep_MeshChanged();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ItemID '%s' not found in DataTable."), *ItemID.ToString());
	}
}

// Called when the game starts or when spawned
void ANGDropItemActor::BeginPlay()
{
	Super::BeginPlay();
}

void ANGDropItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANGDropItemActor, ReplicatedMesh);
}


void ANGDropItemActor::OnRep_MeshChanged()
{
	if (MeshComponent && ReplicatedMesh)
	{
		MeshComponent->SetStaticMesh(ReplicatedMesh);
	}
}
