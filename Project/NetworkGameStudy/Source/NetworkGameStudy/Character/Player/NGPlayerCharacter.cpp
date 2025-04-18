// Copyright Epic Games, Inc. All Rights Reserved.

#include "NGPlayerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Character/Animation/NGAnimInstance.h"
#include "EngineUtils.h"
#include "GameFramework/GameStateBase.h"
#include "NetworkGameStudy.h"
#include "Engine/OverlapResult.h"
#include "Engine/DamageEvents.h"
#include "Net/UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////////
// ANGPlayerCharacter

ANGPlayerCharacter::ANGPlayerCharacter()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	WeaponMeshComponent->SetupAttachment(RootComponent);
	// 우선 무기는 임시로 붙입니다.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Silly_Weapons/Blade_Balloon/SK_Blade_Balloon.SK_Blade_Balloon'"));
	if (nullptr != WeaponMeshRef.Object)
	{
		WeaponMesh = WeaponMeshRef.Object;

		USkeletalMeshComponent* CharacterMesh = GetMesh(); // 주로 캐릭터의 본체

		// 메시 세팅
		WeaponMeshComponent->SetSkeletalMesh(WeaponMesh);

		// 부모 설정 + 소켓에 붙이기
		WeaponMeshComponent->AttachToComponent(CharacterMesh, FAttachmentTransformRules::KeepRelativeTransform, FName("waepon_socket"));
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PalSphereMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/00_Game/Character/Weapon/Mesh/Shape_Sphere.Shape_Sphere'"));

	if (nullptr != PalSphereMeshRef.Object)
		PalSphereMesh = PalSphereMeshRef.Object;
}

void ANGPlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void ANGPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ANGPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANGPlayerCharacter, TargetFocusStack_Rep);
	DOREPLIFETIME(ANGPlayerCharacter, CurrentWeaponMeshName);
	DOREPLIFETIME(ANGPlayerCharacter, IsPalSphereLockOn);
}

void ANGPlayerCharacter::InitializeStatComponent()
{
	if (UUIManagerWorldSubsystem* UIManager = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>())
	{
		NGStatComponent->Initialize(UIManager->GeMainWidgetInstance());
	}
}

void ANGPlayerCharacter::OnRep_ChangedFocusStack()
{
	if (TargetFocusStack_Rep.IsEmpty())
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	else
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void ANGPlayerCharacter::OnRep_ChangedWeaponMesh()
{
	// 우선 임의로 상수를 넣어서 동작하게 만듭니다.
	if (CurrentWeaponMeshName == TEXT("Sword"))
	{
		WeaponMeshComponent->SetSkeletalMesh(WeaponMesh);
	}
	else if (CurrentWeaponMeshName == TEXT("PalSphere"))
	{
		WeaponMeshComponent->SetSkeletalMesh(PalSphereMesh);
	}
	else
	{
		WeaponMeshComponent->SetSkeletalMesh(nullptr);
	}
}


void ANGPlayerCharacter::OnRep_ChangedIsPalSphereLockOn()
{
	NG_LOG(LogTemp, Log, TEXT("Begin"));

	if (IsPalSphereLockOn)
		GetCachedAnimInstance()->PlayLockOn();
	else
		GetCachedAnimInstance()->StopLockOn();
}

//////////////////////////////////////////////////////////////////////////
// Input
void ANGPlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ANGPlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ANGPlayerCharacter::AttackTriggered(const FInputActionValue& Value)
{
	if (!IsPalSphereLockOn)
	{
		Attack_Implementation();
	}
	else
	{
		ThrowPalSphere();
	}
}

void ANGPlayerCharacter::AttackStarted(const FInputActionValue& Value)
{
	ServerRPCTryFocusTarget(true, TEXT("ANGPlayerCharacter::AttackStarted"));
}

void ANGPlayerCharacter::AttackCompleted(const FInputActionValue& Value)
{
	ServerRPCTryFocusTarget(false, TEXT("ANGPlayerCharacter::AttackCompleted"));
}

void ANGPlayerCharacter::LockOn(const FInputActionValue& Value)
{
	NG_LOG(LogTemp, Log, TEXT("Begin"));
	ServerRPCSetIsPalSphereLockOn(true);
	ServerRPCTryFocusTarget(true, TEXT("ANGPlayerCharacter::LockOn"));
	ServerRPCChangeWeaponMesh(TEXT("PalSphere"));
}

void ANGPlayerCharacter::LockOnCancel(const FInputActionValue& Value)
{
	NG_LOG(LogTemp, Log, TEXT("Begin"));
	ServerRPCSetIsPalSphereLockOn(false);
	ServerRPCTryFocusTarget(false, TEXT("ANGPlayerCharacter::LockOnCancel"));
	ServerRPCChangeWeaponMesh(TEXT("Sword"));
}

void ANGPlayerCharacter::ServerRPCTryFocusTarget_Implementation(bool InUseControllerRotationYaw, FName InDebugName)
{
	if (InUseControllerRotationYaw)
	{
		TargetFocusStack_Rep.Add(InDebugName);
	}
	else
	{
		// 생각해보니 들어오는 순서대로 나갈 일이 없으니..
		// 자세하게 디버깅하고 싶다면 맵 구조로 만들자
		FName DebugName = TargetFocusStack_Rep.Pop();
		// ensure(DebugName == InDebugName);
	}

	// 서버에도 반영이 필요합니다.
	OnRep_ChangedFocusStack();
}

void ANGPlayerCharacter::ServerRPCSetIsPalSphereLockOn_Implementation(bool bLockOn)
{
	IsPalSphereLockOn = bLockOn;
}

void ANGPlayerCharacter::ServerRPCChangeWeaponMesh_Implementation(FName WeaponName)
{
	CurrentWeaponMeshName = WeaponName;

	OnRep_ChangedWeaponMesh();
}

void ANGPlayerCharacter::ServerRPCSpawnPalSphere_Implementation()
{
	NG_LOG(LogTemp, Warning, TEXT("!!!!Spawn!!!!"));

	// 서버에서 팰 스피어를 생성합니다.
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.f + FVector(0.0f, 0.0f, 50.0f);
	FRotator SpawnRotation = GetControlRotation();
	// 팰스피어를 생성합니다.
	ANGWeaponPalSphere* PalSphere = GetWorld()->SpawnActor<ANGWeaponPalSphere>(ANGWeaponPalSphere::StaticClass(), SpawnLocation, SpawnRotation);
	if (PalSphere)
		PalSphere->SetOwner(this);
}



void ANGPlayerCharacter::ThrowPalSphere()
{
	if (!bCanAttack) return;

	bCanAttack = false;

	// AttackTime 후에 공격 가능 여부를 초기화 시킵니다.
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ANGPlayerCharacter::ResetAttack, AttackTime, false);

	ServerRPC_PlayAnim(TEXT("Throw"));
}
