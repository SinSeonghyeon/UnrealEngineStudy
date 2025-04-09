// Fill out your copyright notice in the Description page of Project Settings.


#include "NGBreakableProp.h"
#include "../NetworkGameStudy.h"
#include "../Item/NGDropItemActor.h"

// Sets default values
ANGBreakableProp::ANGBreakableProp()
{
	CurrentHP = 100;

	PropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PropMesh"));
	PropMesh->SetupAttachment(RootComponent);

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

	FVector SpawnLocation = GetActorLocation() + FMath::VRand() * 30.f + FVector(0.0f, 100.0f, 0.0f);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	ANGDropItemActor* Dropped = World->SpawnActor<ANGDropItemActor>(ANGDropItemActor::StaticClass(), SpawnLocation, SpawnRotation);
	Dropped->Initialize(TEXT("000"));

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

void ANGBreakableProp::MulticastRPCShakeProp_Implementation()
{
	// AttackTime 후에 공격 가능 여부를 초기화 시킵니다.
	GetWorldTimerManager().SetTimer(ShakeHandle, this, &ANGBreakableProp::ShakeProp, 0.016f, true);
}

// 해당 코드는 서버에서 실행될 예정입니다.
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
