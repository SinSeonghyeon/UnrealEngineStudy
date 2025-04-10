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

// Called when the game starts or when spawned
void ANGDropItemActor::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ANGDropItemActor::OnMeshOverlapBegin);
	MeshComponent->OnComponentEndOverlap.AddDynamic(this, &ANGDropItemActor::OnMeshOverlapEnd);
}

void ANGDropItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANGDropItemActor, ReplicatedMesh);
}

void ANGDropItemActor::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	NG_LOG(LogTemp, Log, TEXT("OverlapBegin"));
}

void ANGDropItemActor::OnMeshOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	NG_LOG(LogTemp, Log, TEXT("OverlapEnd"));
}

void ANGDropItemActor::OnRep_MeshChanged()
{
	if (MeshComponent && ReplicatedMesh)
	{
		MeshComponent->SetStaticMesh(ReplicatedMesh);
	}
}
