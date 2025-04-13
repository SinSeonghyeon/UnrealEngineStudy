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

	// PrimaryActorTick.bCanEverTick = true;
}

void ANGWeaponPalSphere::BeginPlay()
{
	Super::BeginPlay();

	// 서버에서 전달받도록
	if(GetNetMode() != ENetMode::NM_Client)
		CollisionComponent->OnComponentHit.AddDynamic(this, &ANGWeaponPalSphere::OnHit);

}

void ANGWeaponPalSphere::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		// NG_LOG(LogTemp, Warning, TEXT("Hit: %s"), *OtherActor->GetName());
	}
}

