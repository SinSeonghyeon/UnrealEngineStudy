// Fill out your copyright notice in the Description page of Project Settings.


#include "NGDropItemActor.h"
#include "../NGGameInstance.h"
#include "ItemData.h"
#include "Net/UnrealNetwork.h"
#include "../NetworkGameStudy.h"
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

// 서버에서 호출됩니다.
void ANGDropItemActor::DoIneraction(ANGPlayerController* PlayerController)
{
	// 서버에도 아이템을 같이 업데이트 하는 이유는 추후에 아이템을 사용할 시점에 유효성 검사를 하기 위함입니다.
	PlayerController->ClientRPCAddItem(ItemID);
	PlayerController->ClientRPCAddItem_Implementation(ItemID);

	Destroy();
}

// Called when the game starts or when spawned
void ANGDropItemActor::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() == ENetMode::NM_Client)
	{
		MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ANGDropItemActor::OnInteractionOverlapBegin);
		MeshComponent->OnComponentEndOverlap.AddDynamic(this, &ANGDropItemActor::OnInteractionOverlapEnd);
	}
}

void ANGDropItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANGDropItemActor, Mesh_Rep);
}

void ANGDropItemActor::OnRep_MeshChanged()
{
	if (MeshComponent && Mesh_Rep)
	{
		MeshComponent->SetStaticMesh(Mesh_Rep);
	}
}
