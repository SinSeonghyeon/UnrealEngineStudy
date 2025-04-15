// Fill out your copyright notice in the Description page of Project Settings.


#include "NGWeaponPalSphere.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ANGWeaponPalSphere::ANGWeaponPalSphere()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetCollisionProfileName(TEXT("BlockAll"));
	CollisionComponent->SetSphereRadius(25.0f);
	RootComponent = CollisionComponent;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(CollisionComponent);
	SkeletalMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/00_Game/Character/Weapon/Mesh/Shape_Sphere.Shape_Sphere'"));
	if (nullptr != MeshAsset.Object)
	{
		SkeletalMeshComponent->SetSkeletalMesh(MeshAsset.Object);
	}

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bShouldBounce = true;

	// 발사시 조절해야되는 변수들.
	ProjectileMovementComponent->InitialSpeed = 2000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->Velocity = FVector(1.0f, 0.0f, 0.01f);
	ProjectileMovementComponent->ProjectileGravityScale = 1.0f;

	bReplicates = true;

	CurrentShakeTimeRemaining = ShakeTimeRemaining;
	CurrentWaitTime = ShakeTimeRemaining;
	// PrimaryActorTick.bCanEverTick = true;
}

void ANGWeaponPalSphere::BeginPlay()
{
	Super::BeginPlay();

	// 서버에서 전달받도록
	if(GetNetMode() != ENetMode::NM_Client)
		CollisionComponent->OnComponentHit.AddDynamic(this, &ANGWeaponPalSphere::OnHit);

	GetWorldTimerManager().SetTimer(DestroyTimeHandle, this, &ANGWeaponPalSphere::DestroyPalSphere, 3.0f, false);
}

void ANGWeaponPalSphere::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bIsCapturing) return;


	if (OtherActor && OtherActor->ActorHasTag(TEXT("PalCharacter")))
	{
		NG_LOG(LogTemp, Warning, TEXT("Hit: %s"), *OtherActor->GetName());

		MultiCastRPCStartCaptureSequence(Cast<ANGPalCharacter>(OtherActor));
	}
}

void ANGWeaponPalSphere::DestroyPalSphere()
{
	Destroy();
}


void ANGWeaponPalSphere::MultiCastRPCStartCaptureSequence_Implementation(ANGPalCharacter* InTargetPal)
{
	TargetPal = InTargetPal;
	TargetPal->MulticastRPCTryCapture();
	// 움직임 제거
	ProjectileMovementComponent->StopMovementImmediately();
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	CollisionComponent->SetSimulatePhysics(false);
	CollisionComponent->SetCollisionProfileName(TEXT("NoCollision"));

	bIsCapturing = true;

	GetWorldTimerManager().ClearTimer(DestroyTimeHandle);

	if (GetNetMode() == ENetMode::NM_Client)
	{
		InitialLocation = GetActorLocation();

		float RandomAngle = FMath::FRandRange(0.f, 2.f * PI);
		FVector Offset = { FMath::Cos(RandomAngle) * 100.0f, FMath::Sin(RandomAngle) * 100.0f, FMath::FRandRange(200.0f, 300.0f) };
		DestinationLocation = InitialLocation + Offset;

		CurrentWaitTime = ShakeTimeRemaining;
		CurrentShakeTimeRemaining = ShakeTimeRemaining;
		CurrentInitialTimer = InitialTime;
		MaxShakeCount = FMath::RandRange(3, 5);
		CurrentShakeCount = 0;
		OriginalRotation = GetActorRotation();

		GetWorldTimerManager().SetTimer(CaptureShakeHandle, this, &ANGWeaponPalSphere::PlayCaptureShake, 0.016f, true);
	}
}

void ANGWeaponPalSphere::PlayCaptureShake()
{
	const float DeltaTime = 0.016f;
	if (CurrentShakeCount > MaxShakeCount)
	{
		// 흔들림 중지
		GetWorldTimerManager().ClearTimer(CaptureShakeHandle);

		ServerRPCEvaluateCaptureResult();
	}
	else
	{
		// 흔들림이 시작하는 위치로 이동합니다.
		if (CurrentInitialTimer > 0.0f)
		{
			CurrentInitialTimer -= DeltaTime;
			float t = CurrentInitialTimer / InitialTime;

			FVector CurrentLocation = FMath::Lerp(DestinationLocation, InitialLocation, t);

			SetActorLocation(CurrentLocation);
		}
		// 도착 후 흔들림이 시작됩니다.
		else if (CurrentShakeTimeRemaining > 0.0f)
		{
			CurrentShakeTimeRemaining -= DeltaTime;

			float t = CurrentShakeTimeRemaining / ShakeTimeRemaining;

			float CurrentShakeStrength = FMath::Lerp(ShakeStrength, 0, 1 - t);
			float CurrentShakeSpeed = FMath::Lerp(ShakeSpeed, 0, t);

			float ShakeAngle = FMath::Sin(DeltaTime * CurrentShakeSpeed) * CurrentShakeStrength;
			FRotator NewRotation = OriginalRotation + FRotator(0.f, 0.f, ShakeAngle);
			SetActorRotation(NewRotation);

			if (CurrentShakeTimeRemaining <= 0.f)
			{
				SetActorRotation(OriginalRotation); // 원위치
			}
		}
		else if (CurrentWaitTime > 0.0f)
		{
			CurrentWaitTime -= DeltaTime;
		}
		else
		{
			CurrentWaitTime = ShakeTimeRemaining;
			CurrentShakeTimeRemaining = ShakeTimeRemaining;
			CurrentShakeCount++;
		}
	}
}

void ANGWeaponPalSphere::ServerRPCEvaluateCaptureResult_Implementation()
{
	bCaptureSuccessed = FMath::FRandRange(0.0f, 100.0f) < 20.0f; // 20% 확률 서버에서 포획 성공 여부를 결정.

	if (bCaptureSuccessed)
	{
		// 플레이어에게 펠을 지급
		TargetPal->SuccessCapture();
	}
	else
	{
		// 펠을 다시 스폰.
		TargetPal->MulticastRPCCancelCapture();
	}
	DestroyPalSphere();
}

