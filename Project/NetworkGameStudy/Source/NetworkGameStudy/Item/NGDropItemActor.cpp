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

void ANGDropItemActor::DoIneraction()
{
	// 플레이어에게 아이템 지급 필요.
	NG_LOG(LogTemp, Log, TEXT("Begin"));

	if (ANGPlayerController* Controller = Cast<ANGPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		Controller->ServerRPCRequestDestroyActor(this);
	}
}

// Called when the game starts or when spawned
void ANGDropItemActor::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ANGDropItemActor::OnInteractionOverlapBegin);
	MeshComponent->OnComponentEndOverlap.AddDynamic(this, &ANGDropItemActor::OnInteractionOverlapEnd);
}

void ANGDropItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANGDropItemActor, Mesh_Rep);
}

void ANGDropItemActor::OnInteractionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnInteractionOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	NG_LOG(LogTemp, Log, TEXT("OverlapBegin"));
}

void ANGDropItemActor::OnInteractionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnInteractionOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	NG_LOG(LogTemp, Log, TEXT("OverlapEnd"));
}

void ANGDropItemActor::OnRep_MeshChanged()
{
	if (MeshComponent && Mesh_Rep)
	{
		MeshComponent->SetStaticMesh(Mesh_Rep);
	}
}
