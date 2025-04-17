// Copyright Epic Games, Inc. All Rights Reserved.

#include "NGPalCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISense_Damage.h"
#include "../NGGameInstance.h"
#include "Components/WidgetComponent.h"
#include "../UI/NGHeadUpWidget.h"


//////////////////////////////////////////////////////////////////////////
// ANGPalCharacter

const FName PalCollisionProfileFName = TEXT("Pawn");

ANGPalCharacter::ANGPalCharacter()
{
	bUseControllerRotationYaw = true;

	// 클래스 로드
	static ConstructorHelpers::FClassFinder<UUserWidget> HeadUpWidgetBPClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/00_Game/UI/BP_HeadUpWidget.BP_HeadUpWidget_C'"));
	if (HeadUpWidgetBPClass.Succeeded())
	{
		HeadUpWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HeadUpWidgetComponent"));
		HeadUpWidgetComponent->SetupAttachment(RootComponent);
		HeadUpWidgetComponent->SetDrawSize(FVector2D(200.0f, 10.0f));
		HeadUpWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		HeadUpWidgetComponent->SetWidgetClass(HeadUpWidgetBPClass.Class);
		HeadUpWidgetComponent->SetVisibility(false);
	}

	GetCapsuleComponent()->SetCollisionProfileName(PalCollisionProfileFName);

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

	if (HeadUpWidgetComponent)
		HeadUpWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 30.0f));
}

void ANGPalCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (CachedPalController = Cast<ANGPalController>(NewController))
	{
		CachedPalController->OnFindTarget.AddUObject(this, &ANGPalCharacter::OnFindTarget);
		CachedPalController->OnLostTarget.AddUObject(this, &ANGPalCharacter::OnLostTarget);
	}
}

void ANGPalCharacter::UnPossessed()
{
	CachedPalController->OnFindTarget.RemoveAll(this);
	CachedPalController->OnLostTarget.RemoveAll(this);
}

void ANGPalCharacter::InitializeStatComponent()
{
	if (HeadUpWidgetComponent)
	{
		NGStatComponent->Initialize(Cast<UNGStatWidgetBase>(HeadUpWidgetComponent->GetUserWidgetObject()));
	}
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
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
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
	GetCapsuleComponent()->SetCollisionProfileName(PalCollisionProfileFName);
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

void ANGPalCharacter::OnFindTarget()
{
	MulticastRPCSetVisibilityHeadupWidget(true);
}

void ANGPalCharacter::OnLostTarget()
{
	MulticastRPCSetVisibilityHeadupWidget(false);
}

void ANGPalCharacter::MulticastRPCSetVisibilityHeadupWidget_Implementation(bool bVisibility)
{
	NG_LOG(LogTemp, Log, TEXT("Begin"));
	if (HeadUpWidgetComponent)
	{
		HeadUpWidgetComponent->SetVisibility(bVisibility);
	}
}