// Copyright Epic Games, Inc. All Rights Reserved.

#include "NGPalCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISense_Damage.h"
#include "../NGGameInstance.h"


//////////////////////////////////////////////////////////////////////////
// ANGPalCharacter

ANGPalCharacter::ANGPalCharacter()
{
	bUseControllerRotationYaw = true;
	
	Tags.Add(TEXT("PalCharacter"));
}

float ANGPalCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float AcualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UAISense_Damage::ReportDamageEvent(
		GetWorld(),
		this,
		DamageCauser,
		AcualDamage,
		DamageCauser->GetActorLocation(),
		GetActorLocation());

	return AcualDamage;
}

void ANGPalCharacter::BeginPlay()
{
	Super::BeginPlay();

	CachedMaterials = GetMesh()->GetMaterials();

	OriginMeshScale = GetMesh()->GetRelativeScale3D();
}

void ANGPalCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	CachedPalController = Cast<ANGPalController>(NewController);
}

void ANGPalCharacter::UpdateMaxWalkSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void ANGPalCharacter::MulticastRPCTryCapture_Implementation()
{
	if (GetNetMode() != ENetMode::NM_Client)
	{
		CachedPalController->PauseBehaviorTree();
	}

	// 서버에서는 동작할필요 없지만, 같이 없애주지 않으면 서버의 콜라이더로 인해 이상한 현상이 발생합니다.
	// 임시로 클라와 같은 동작을 수행하도록 변경합니다.
	StartCapture();
}

void ANGPalCharacter::MulticastRPCCancelCapture_Implementation()
{
	if (GetNetMode() != ENetMode::NM_Client)
	{
		CachedPalController->ResumeBehaviorTree();
	}

	// 서버에서는 동작할필요 없지만, 같이 없애주지 않으면 서버의 콜라이더로 인해 이상한 현상이 발생합니다.
	// 임시로 클라와 같은 동작을 수행하도록 변경합니다.
	StartEscape();
}

void ANGPalCharacter::SuccessCapture()
{
	Destroy();
}

void ANGPalCharacter::StartCapture()
{
	CaptureTimer = InitTime;

	TObjectPtr<UMaterial> EmssiveMeterial = UNGGameInstance::GetEmissiveMaterial(GetWorld());

	for (int32 i = 0; i < CachedMaterials.Num(); ++i)
	{
		GetMesh()->SetMaterial(i, EmssiveMeterial);
	}
	GetCharacterMovement()->SetComponentTickEnabled(false);
	GetWorldTimerManager().SetTimer(CaptureHandle, this, &ANGPalCharacter::PlayCapture, DeltaTime, true);
}

void ANGPalCharacter::PlayCapture()
{
	if (CaptureTimer > 0.0f)
	{
		CaptureTimer -= DeltaTime;
		float t = CaptureTimer / InitTime;
		SetActorScale3D(FVector(t));
	}
	else
	{
		SetActorScale3D(FVector(0.0f));
		GetWorldTimerManager().ClearTimer(CaptureHandle);
	}
}

void ANGPalCharacter::StartEscape()
{
	SetActorScale3D(FVector(0.01f));
	GetMesh()->SetRelativeScale3D(OriginMeshScale);
	EscapeTimer = InitTime;
	GetWorldTimerManager().SetTimer(EscapeHandle, this, &ANGPalCharacter::PlayEscape, DeltaTime, true);
}

void ANGPalCharacter::PlayEscape()
{
	if (EscapeTimer > 0.0f)
	{
		EscapeTimer -= DeltaTime;
		float t = EscapeTimer / InitTime;
		SetActorScale3D(FVector(1.0f - t));
	}
	else
	{
		SetActorScale3D(FVector(1.0f));

		GetCharacterMovement()->SetComponentTickEnabled(true);
		// 필요 시 상태 전이 조건 재평가
		GetCachedAnimInstance()->NativeUpdateAnimation(0.0f); // 강제로 1프레임 평가

		GetWorldTimerManager().ClearTimer(EscapeHandle);
		for (int32 i = 0; i < CachedMaterials.Num(); ++i)
		{
			GetMesh()->SetMaterial(i, CachedMaterials[i]);
		}
	}
}
