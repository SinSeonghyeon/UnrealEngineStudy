// Fill out your copyright notice in the Description page of Project Settings.


#include "NGBreakableProp.h"
#include "../NetworkGameStudy.h"
#include "../Item/NGDropItemActor.h"
#include "../NGGameInstance.h"
#include "../Item/ItemData.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
ANGBreakableProp::ANGBreakableProp()
{
	CurrentHP = 100;

	PropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PropMesh"));
	PropMesh->SetupAttachment(RootComponent);

	DestructibleMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollectionComponent"));
	DestructibleMesh->SetCollisionProfileName(TEXT("Breakable"));
	DestructibleMesh->AttachToComponent(PropMesh, FAttachmentTransformRules::KeepRelativeTransform);
	DestructibleMesh->SetSimulatePhysics(false);
	DestructibleMesh->SetVisibility(false);
	DestructibleMesh->DamageThreshold.Empty();
	DestructibleMesh->DamageThreshold.Add(0);

	bReplicates = true;
}

// Called when the game starts or when spawned
void ANGBreakableProp::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotation = GetActorRotation();
	CurrentShakeTimeRemaining = ShakeTimeRemaining;
}

void ANGBreakableProp::BreakProp()
{
	NG_LOG(LogNGNetwork, Log, TEXT("Begin"));

	SpawnDropItem();

	MulticastRPCDestroyMesh();
	
	// 서버에서 액터를 삭제합니다. 클라에 동기화가 맞춰집니다.
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ANGBreakableProp::DestroyActor, 2.0f, false);
}

void ANGBreakableProp::ShakeProp()
{
	if (CurrentShakeTimeRemaining > 0.f)
	{
		CurrentShakeTimeRemaining -= 0.016;

		float Factor = CurrentShakeTimeRemaining / ShakeTimeRemaining;

		float CurrentShakeStrength = FMath::Lerp(ShakeStrength, 0, 1 - Factor);
		float CurrentShakeSpeed = FMath::Lerp(ShakeSpeed, 0, Factor);

		float ShakeAngle = FMath::Sin(0.016f * CurrentShakeSpeed) * CurrentShakeStrength;
		FRotator NewRotation = OriginalRotation + FRotator(0.f, 0.f, ShakeAngle);
		SetActorRotation(NewRotation);

		if (CurrentShakeTimeRemaining <= 0.f)
		{
			SetActorRotation(OriginalRotation); // 원위치
		}
	}
	else
	{
		CurrentShakeTimeRemaining = ShakeTimeRemaining;
		GetWorldTimerManager().ClearTimer(ShakeHandle);
	}
}

void ANGBreakableProp::DestroyActor()
{
	NG_LOG(LogTemp, Log, TEXT("Begin"));
	Destroy();
}

void ANGBreakableProp::SpawnDropItem()
{
	UWorld* World = GetWorld();
	if (!World) return;

	FVector Origin, BoxExtent;
	GetActorBounds(true, Origin, BoxExtent);

	float RandomAngle = FMath::FRandRange(0.f, 2.f * PI);
	FVector Offset = { FMath::Cos(RandomAngle) * SpawnRadius, FMath::Sin(RandomAngle) * SpawnRadius, FMath::FRandRange(SpawnHeightMin, SpawnHeightMax) };
	FVector SpawnLocation = GetActorLocation() + Offset;
	FRotator SpawnRotation = FRotator::ZeroRotator;

	if (!DropItmeIDs.IsEmpty())
	{
		const FItemData* ItemData = UNGGameInstance::GetItemDataTableManager(GetWorld())->GetItemData(DropItmeIDs[FMath::RandRange(0, DropItmeIDs.Num() - 1)]);

		ANGDropItemActor* Dropped = World->SpawnActor<ANGDropItemActor>(ItemData->DropActorClass, SpawnLocation, SpawnRotation);

		if (Dropped)
			UE_LOG(LogTemp, Log, TEXT("Dropped item: %s"), *Dropped->GetName());
	}
}

void ANGBreakableProp::MulticastRPCDestroyMesh_Implementation()
{
	NG_LOG(LogTemp,Log,TEXT("Begin"));

	PropMesh->SetVisibility(false);
	PropMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 서버에서는 해당 함수가 실행될 필요가 없습니다.
	if (GetNetMode() == NM_Client)
	{
		DestructibleMesh->SetVisibility(true);
		DestructibleMesh->SetSimulatePhysics(true);
		DestructibleMesh->AddForce(FVector(0.0f, 0.0f, -1.0f));
	}
}

void ANGBreakableProp::MulticastRPCShakeProp_Implementation()
{
	// 서버에서는 나무를 흔들필요가 없습니다.
	if (GetNetMode() == NM_Client)
	{
		// AttackTime 후에 공격 가능 여부를 초기화 시킵니다.
		GetWorldTimerManager().SetTimer(ShakeHandle, this, &ANGBreakableProp::ShakeProp, 0.016f, true);
	}
}

// 해당 코드는 서버에서 실행되고 있습니다!!
float ANGBreakableProp::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	NG_LOG(LogNGNetwork, Log, TEXT("Begin"));

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHP -= ActualDamage;

	if (CurrentHP <= 0)
	{
		BreakProp();
	}
	else
	{
		MulticastRPCShakeProp();

		// 파괴되지 않을때도 50퍼의 확률로 랜덤 스폰합니다.
		if(FMath::FRandRange(0.0f, 100.0f) > 50.0f)
			SpawnDropItem();
	}

	return ActualDamage;
}
