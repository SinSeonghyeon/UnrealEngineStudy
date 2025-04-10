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

	UWorld* World = GetWorld();
	if (!World) return;

	FVector SpawnLocation = GetActorLocation() + FMath::VRand() * 100.f + FVector(0.0f, 0.0f, FMath::FRandRange(100.f, 200.f));
	FRotator SpawnRotation = FRotator::ZeroRotator;

	const FItemData* ItemData = UNGGameInstance::GetItemDataTableManager(this)->GetItemData(DropTimeIDs[FMath::RandRange(0, DropTimeIDs.Num() - 1)]);

	ANGDropItemActor* Dropped = World->SpawnActor<ANGDropItemActor>(ItemData->DropActorClass, SpawnLocation, SpawnRotation);
	
	MulticastRPCDestroyMesh();
	

	if (Dropped)
	{
		UE_LOG(LogTemp, Log, TEXT("Dropped item: %s"), *Dropped->GetName());
	}
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
	Destroy();
}

void ANGBreakableProp::MulticastRPCDestroyMesh_Implementation()
{
	PropMesh->SetVisibility(false);
	// PropMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DestructibleMesh->SetVisibility(true);
	DestructibleMesh->SetSimulatePhysics(true);
	DestructibleMesh->AddForce(FVector(0.0f, 0.0f, -1.0f));

	// 서버로 옮겨줘야함. 테스트를 위해 추가.
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ANGBreakableProp::DestroyActor, 1.0f, false);
}

void ANGBreakableProp::MulticastRPCShakeProp_Implementation()
{
	// AttackTime 후에 공격 가능 여부를 초기화 시킵니다.
	GetWorldTimerManager().SetTimer(ShakeHandle, this, &ANGBreakableProp::ShakeProp, 0.016f, true);
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
	}

	return ActualDamage;
}
