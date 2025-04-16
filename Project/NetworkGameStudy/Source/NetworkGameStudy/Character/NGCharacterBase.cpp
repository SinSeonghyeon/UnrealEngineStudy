// Copyright Epic Games, Inc. All Rights Reserved.

#include "NGCharacterBase.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Animation/NGAnimInstance.h"
#include "EngineUtils.h"
#include "GameFramework/GameStateBase.h"
#include "../NetworkGameStudy.h"
#include "Engine/OverlapResult.h"
#include "Engine/DamageEvents.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ANGCharacterBase

ANGCharacterBase::ANGCharacterBase()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	NGStatComponent = CreateDefaultSubobject<UNGStatComponent>(TEXT("NGStatComponent"));
	NGStatComponent->OnHealthZero.AddUObject(this, &ANGCharacterBase::DieCharacter);
}

void ANGCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (USkeletalMeshComponent* CoreMesh = GetMesh())
	{
		CachedAnimInstance = Cast<UNGAnimInstance>(CoreMesh->GetAnimInstance());
	}
}

void ANGCharacterBase::Attack_Implementation()
{
	if (!bCanAttack) return;

	// 플레이어의 경우 해당 분기를 타게됩니다.
	if (GetNetMode() == ENetMode::NM_Client)
	{
		bCanAttack = false;

		// AttackTime 후에 공격 가능 여부를 초기화 시킵니다.
		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ANGCharacterBase::ResetAttack, AttackTime, false);

		// 애니메이션을 서버에서 재생하지 않고 클라에서 하는 이유는 반응성을 빠르게 하기 위함입니다.
		CachedAnimInstance->PlayAttackMontage();

		// 공격을 했다는 사실을 서버에 알립니다.
		ServerRPCAttack_PlayAnim(GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
	}
	else
	{
		// 서버에서 호출되는 경우는 팰들이 공격할 때입니다.
		MultiCastRPCPlayAttack_PlayAnim();
	}
}

void ANGCharacterBase::ResetAttack()
{
	bCanAttack = true;
}

void ANGCharacterBase::ServerRPCAttack_HitCheck_Implementation(FVector StartPos, float Radius)
{
	TArray<FOverlapResult> HitResults;

	FDamageEvent DamageEvent;

	bool bHit = GetWorld()->OverlapMultiByObjectType(HitResults, StartPos, FQuat::Identity, FCollisionObjectQueryParams::AllObjects, FCollisionShape::MakeSphere(Radius));

	// 클라에 던져주나? 클라에도 던져줍니다.
	// DrawDebugSphere(GetWorld(), StartPos, Radius, 18, FColor::Red, false, 1.0f);

	if (bHit)
	{
		for (const FOverlapResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (this != HitActor)
			{
				HitActor->TakeDamage(NGStatComponent->GetDamage(), DamageEvent, this->GetController(), this);
			}
		}
	}
}

void ANGCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeStatComponent();
}

// 이 함수는 서버에서 호출되고 있습니다.
float ANGCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	MultiCastRPCPlayHit_PlayAnim();

	NGStatComponent->SetCurrentHP(NGStatComponent->GetCurrentHP() - ActualDamage);

	return ActualDamage;
}

void ANGCharacterBase::DieCharacter()
{
	// 사망 관련 코드 추가 필요.
	// 렉돌
	// 컨트롤러 뻇기.
	// 콜리전 제거 등..
	// UI 제거..
}

void ANGCharacterBase::ServerRPCAttack_PlayAnim_Implementation(float AttackStartTime)
{
	LastAttackStartTime = AttackStartTime;

	// 자신을 제외한 나머지 플레이어에게 애니메이션 재생 RPC를 던집니다.
	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		// RPC를 호출한 PlayerController를 빼고 RPC를 던진다.
		if (PlayerController && GetController() != PlayerController)
		{
			ANGCharacterBase* OtherPlayer = Cast<ANGCharacterBase>(PlayerController->GetPawn());
			if (OtherPlayer)
			{
				OtherPlayer->ClientRPCPlayAttack_PlayAnim(this);
			}
		}
	}
}

bool ANGCharacterBase::ServerRPCAttack_PlayAnim_Validate(float AttackStartTime)
{
	if (LastAttackStartTime == 0.0f)
	{
		return true;
	}

	return (AttackStartTime - LastAttackStartTime) > (AttackTime - 0.4f);
}

void ANGCharacterBase::ServerRPC_PlayAnim_Implementation(FName AnimName)
{
	MultiCastRPCPlay_PlayAnim(AnimName);
}

void ANGCharacterBase::MultiCastRPCPlay_PlayAnim_Implementation(FName AnimName)
{
	GetCachedAnimInstance()->PlayAnimFromName(AnimName);
}

// 다른 캐릭터의 애니메이션을 재생하는 것이 조금 이상하긴 하지만..
// 이렇게 하지 않으면 어떻게 처리해야하는가?
// 변수 리플리케이션에 함수를 바인딩 시키는 것 밖에 없을 듯.
// 하지만 그럼 공격 시작, 끝 두번 보내니 한번 보내는 이게 나을 것 같습니다.
void ANGCharacterBase::ClientRPCPlayAttack_PlayAnim_Implementation(ANGCharacterBase* CharacterToPlay)
{
	NG_LOG(LogNGNetwork, Log, TEXT("Begin"));

	CharacterToPlay->GetCachedAnimInstance()->PlayAttackMontage();
}

void ANGCharacterBase::MultiCastRPCPlayAttack_PlayAnim_Implementation()
{
	NG_LOG(LogNGNetwork, Log, TEXT("Begin"));

	bCanAttack = false;

	// AttackTime 후에 공격 가능 여부를 초기화 시킵니다.
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ANGCharacterBase::ResetAttack, AttackTime, false);

	GetCachedAnimInstance()->PlayAttackMontage();
}

void ANGCharacterBase::MultiCastRPCPlayHit_PlayAnim_Implementation()
{
	NG_LOG(LogNGNetwork, Log, TEXT("Begin"));

	GetCachedAnimInstance()->PlayHitMontage();
}
