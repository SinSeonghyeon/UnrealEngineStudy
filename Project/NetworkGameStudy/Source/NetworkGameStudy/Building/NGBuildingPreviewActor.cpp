// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/NGBuildingPreviewActor.h"
#include "Net/UnrealNetwork.h"

ANGBuildingPreviewActor::ANGBuildingPreviewActor()
{
	BuildingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	BuildingMeshComponent->SetCollisionProfileName("NoCollision");

	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("/Script/Engine.Material'/Game/00_Game/Building/M_BuildingPreviewMaterial.M_BuildingPreviewMaterial'"));
	if (nullptr != Material.Object)
	{
		BaseMaterial = Material.Object;
	}

	bReplicates = true;
	SetReplicateMovement(true);
}
// 서버에서 호출합니다.
void ANGBuildingPreviewActor::SetBuildingMesh(UStaticMesh* NewMesh)
{
	BuildingMesh = NewMesh;

	OnRep_ChangedMesh();
}

void ANGBuildingPreviewActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Percent += DeltaTime / 2.0f;

	DynMaterial->SetScalarParameterValue(TEXT("BuildingPercent"), Percent);
}

void ANGBuildingPreviewActor::BeginPlay()
{
	Super::BeginPlay();

	DynMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	BuildingMeshComponent->SetMaterial(0, DynMaterial);
}


void ANGBuildingPreviewActor::DoIneraction()
{
	
}

void ANGBuildingPreviewActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANGBuildingPreviewActor, BuildingMesh);
}

void ANGBuildingPreviewActor::OnRep_ChangedMesh()
{
	BuildingMeshComponent->SetStaticMesh(BuildingMesh);
}
