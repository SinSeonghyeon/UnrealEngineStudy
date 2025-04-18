// Fill out your copyright notice in the Description page of Project Settings.


#include "NGPlayerController.h"
#include "../../SubSystem/UIManagerWorldSubsystem.h"


void ANGPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	NG_LOG(LogTemp, Log, TEXT("Begin"));
}


void ANGPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ANGPlayerController::BeginPlay()
{
	NG_LOG(LogTemp, Log, TEXT("Begin"));

	if (GetNetMode() == ENetMode::NM_Client)
	{
		if (UUIManagerWorldSubsystem* UIManager = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>())
		{
			UIManager->ShowMainUI(this);
		}
	}
}

void ANGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	NG_LOG(LogTemp, Log, TEXT("Begin"));

	check(InputComponent);

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ANGPlayerController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ANGPlayerController::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANGPlayerController::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANGPlayerController::Look);

		// Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ANGPlayerController::AttackStarted);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ANGPlayerController::AttackTriggered);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &ANGPlayerController::AttackCompleted);

		// LockOn
		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &ANGPlayerController::LockOn);
		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Completed, this, &ANGPlayerController::LockOnCancel);

		// LockOn
		EnhancedInputComponent->BindAction(BuildingWidgetToggleAction, ETriggerEvent::Started, this, &ANGPlayerController::ToggleBuildingWidget);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

void ANGPlayerController::Jump()
{
	if (GetPlayerCharacter())
		GetPlayerCharacter()->Jump();
}

void ANGPlayerController::StopJumping()
{
	if (GetPlayerCharacter())
		GetPlayerCharacter()->StopJumping();
}

void ANGPlayerController::Move(const FInputActionValue& Value)
{
	if (GetPlayerCharacter())
		GetPlayerCharacter()->Move(Value);
}

void ANGPlayerController::Look(const FInputActionValue& Value)
{
	if (GetPlayerCharacter())
		GetPlayerCharacter()->Look(Value);
}

void ANGPlayerController::AttackStarted(const FInputActionValue& Value)
{
	if (GetPlayerCharacter())
		GetPlayerCharacter()->AttackStarted(Value);
}

void ANGPlayerController::AttackTriggered(const FInputActionValue& Value)
{
	if (GetPlayerCharacter())
		GetPlayerCharacter()->AttackTriggered(Value);
}

void ANGPlayerController::AttackCompleted(const FInputActionValue& Value)
{
	if (GetPlayerCharacter())
		GetPlayerCharacter()->AttackCompleted(Value);
}

void ANGPlayerController::LockOn(const FInputActionValue& Value)
{
	if (GetPlayerCharacter())
		GetPlayerCharacter()->LockOn(Value);
}

void ANGPlayerController::LockOnCancel(const FInputActionValue& Value)
{
	if (GetPlayerCharacter())
		GetPlayerCharacter()->LockOnCancel(Value);
}

void ANGPlayerController::ToggleBuildingWidget()
{
	if (UUIManagerWorldSubsystem* UIManager = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>())
	{
		UIManager->ToggleBuildingContextUI();
	}
}

TObjectPtr<ANGPlayerCharacter>& ANGPlayerController::GetPlayerCharacter()
{
	if(!PlayerCharacter)
		PlayerCharacter = Cast<ANGPlayerCharacter>(GetPawn());

	return PlayerCharacter;
}
