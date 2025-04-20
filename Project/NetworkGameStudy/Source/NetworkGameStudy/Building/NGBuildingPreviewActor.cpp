// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/NGBuildingPreviewActor.h"
#include "Net/UnrealNetwork.h"
#include "Engine/StaticMeshActor.h"

ANGBuildingPreviewActor::ANGBuildingPreviewActor()
{
	BuildingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	BuildingMeshComponent->SetCollisionProfileName("OverlapOnlyPawn");

	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("/Script/Engine.Material'/Game/00_Game/Building/M_BuildingPreviewMaterial.M_BuildingPreviewMaterial'"));
	if (nullptr != Material.Object)
	{
		BaseMaterial = Material.Object;
		DynMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		BuildingMeshComponent->SetMaterial(0, DynMaterial);
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
}

void ANGBuildingPreviewActor::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() == ENetMode::NM_Client)
	{
		BuildingMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ANGBuildingPreviewActor::OnInteractionOverlapBegin);
		BuildingMeshComponent->OnComponentEndOverlap.AddDynamic(this, &ANGBuildingPreviewActor::OnInteractionOverlapEnd);
	}
}

// 서버에서 호출됩니다.
void ANGBuildingPreviewActor::DoIneraction(ANGPlayerController* PlayerController)
{
	Percent += 0.002f;

	Cast<ANGPlayerCharacter>(PlayerController->GetPawn())->ServerRPCSetPlayBuilindg(true);

	if (Percent > 1.0f)
	{
		AStaticMeshActor* NewActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), GetActorLocation(), GetActorRotation());
		NewActor->SetMobility(EComponentMobility::Movable);
		NewActor->GetStaticMeshComponent()->SetStaticMesh(BuildingMesh);
		NewActor->GetStaticMeshComponent()->SetIsReplicated(true);
		NewActor->SetReplicates(true);

		CancelIneraction(PlayerController);

		Destroy();
	}
}


void ANGBuildingPreviewActor::CancelIneraction(ANGPlayerController* PlayerController)
{
	Cast<ANGPlayerCharacter>(PlayerController->GetPawn())->ServerRPCSetPlayBuilindg(false);
}

void ANGBuildingPreviewActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANGBuildingPreviewActor, BuildingMesh);
	DOREPLIFETIME(ANGBuildingPreviewActor, Percent);
}

void ANGBuildingPreviewActor::OnRep_ChangedMesh()
{
	BuildingMeshComponent->SetStaticMesh(BuildingMesh);
}

void ANGBuildingPreviewActor::OnRep_SetPercent()
{
	if (DynMaterial)
		DynMaterial->SetScalarParameterValue(TEXT("BuildingPercent"), Percent);
}
