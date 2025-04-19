// Fill out your copyright notice in the Description page of Project Settings.


#include "NGPlayerController.h"
#include "../../SubSystem/UIManagerWorldSubsystem.h"


void ANGPlayerController::SetBuildMode(bool bIsSet, const TObjectPtr<UStaticMesh>& BuildingMesh)
{
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (bIsSet)
	{
		ToggleBuildingWidget();
		Subsystem->AddMappingContext(BuildMappingContext, 1);

		ServerRPCSpawnPreviewActor(BuildingMesh);
	}
	else
	{
		Subsystem->RemoveMappingContext(BuildMappingContext);
	}
}

void ANGPlayerController::AddInteractionActor(TWeakObjectPtr<ANGInteractionActorBase> InteractionActor)
{
	if (InteractionActor.IsValid())
	{
		InteractionActorSet.Add(InteractionActor);
		CachedUIManager->SetInteractionWidget(InteractionActor.Get());

		CurrentInteractionActor = InteractionActor;
	}
}

void ANGPlayerController::RemoveInteractionActor(TWeakObjectPtr<ANGInteractionActorBase> InteractionActor)
{
	InteractionActorSet.Remove(InteractionActor);


	if (InteractionActorSet.IsEmpty())
	{
		CachedUIManager->SetInteractionWidget(nullptr);
	}
	else
	{
		TArray<TWeakObjectPtr<ANGInteractionActorBase>> ToRemove;

		if (InteractionActor == CurrentInteractionActor)
		{
			for (auto& Iter : InteractionActorSet)
			{
				if (Iter.IsValid())
				{
					CachedUIManager->SetInteractionWidget(Iter.Get());
					CurrentInteractionActor = *InteractionActorSet.begin();
					break;
				}
				else
				{
					ToRemove.Add(Iter);
				}
			}

			for (auto& Iter : ToRemove)
			{
				InteractionActorSet.Remove(Iter);
			}
		}
	}
	
}

void ANGPlayerController::ServerRPCRequestDestroyActor_Implementation(AActor* Actor)
{
	Actor->Destroy();
}

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

	CachedUIManager = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();

	if (GetNetMode() == ENetMode::NM_Client && CachedUIManager)
	{
		CachedUIManager->ShowMainUI(this);
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

	SetEnhancedInput(Cast<UEnhancedInputComponent>(InputComponent));
}


void ANGPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetNetMode() == ENetMode::NM_Client)
	{
		if (!BuildingPreviewActor)
			return;

		FVector Start;
		FRotator ViewRot;
		GetPlayerViewPoint(Start, ViewRot);

		FVector End = Start + ViewRot.Vector() * 10000.f;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, FCollisionObjectQueryParams::AllStaticObjects, FCollisionQueryParams::DefaultQueryParam))
		{
			ServerRPCMovePreviewActor(Hit.Location);
		}
	}
}

void ANGPlayerController::SetEnhancedInput(TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent)
{
	// Set up action bindings
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
		return;
	}
	// ------------------------ Default ------------------------
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ANGPlayerController::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ANGPlayerController::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANGPlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANGPlayerController::Look);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ANGPlayerController::AttackStarted);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ANGPlayerController::AttackTriggered);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &ANGPlayerController::AttackCompleted);
	EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &ANGPlayerController::LockOn);
	EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Completed, this, &ANGPlayerController::LockOnCancel);
	EnhancedInputComponent->BindAction(BuildingWidgetToggleAction, ETriggerEvent::Started, this, &ANGPlayerController::ToggleBuildingWidget);
	EnhancedInputComponent->BindAction(IntractionAction, ETriggerEvent::Started, this, &ANGPlayerController::Interaction);

	// ------------------------ UI ------------------------
	EnhancedInputComponent->BindAction(UIClickAction, ETriggerEvent::Started, this, &ANGPlayerController::UIClick);
	EnhancedInputComponent->BindAction(UIMouseMoveAction, ETriggerEvent::Triggered, this, &ANGPlayerController::UIMouseMove);

	// ------------------------ Build ------------------------
	EnhancedInputComponent->BindAction(BuildConfirmAction, ETriggerEvent::Started, this, &ANGPlayerController::BuildConfirm);
	EnhancedInputComponent->BindAction(BuildCancleAction, ETriggerEvent::Started, this, &ANGPlayerController::BuildCancle);
}


void ANGPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANGPlayerController, BuildingPreviewActor);
}

void ANGPlayerController::ServerRPCSpawnPreviewActor_Implementation(UStaticMesh* BuildingMesh)
{
	BuildingPreviewActor = GetWorld()->SpawnActor<ANGBuildingPreviewActor>(ANGBuildingPreviewActor::StaticClass());
	BuildingPreviewActor->SetBuildingMesh(BuildingMesh);
}


void ANGPlayerController::ServerRPCDestroyPreviewActor_Implementation()
{
	if (BuildingPreviewActor)
		BuildingPreviewActor->Destroy();
	BuildingPreviewActor = nullptr;
}


void ANGPlayerController::ServerRPCConfirmPreviewActor_Implementation()
{
	BuildingPreviewActor = nullptr;
}


void ANGPlayerController::ServerRPCMovePreviewActor_Implementation(FVector Location)
{
	BuildingPreviewActor->SetActorLocation(Location);
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
	bool bIsOnBuildingContextUI = false;

	bIsOnBuildingContextUI = CachedUIManager->ToggleBuildingContextUI();

	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (bIsOnBuildingContextUI)
	{
		Subsystem->AddMappingContext(UIMappingContext, 1);
	}
	else
	{
		Subsystem->RemoveMappingContext(UIMappingContext);
	}
}


void ANGPlayerController::Interaction()
{
	if(!CurrentInteractionActor.IsValid())
		RemoveInteractionActor(CurrentInteractionActor);

	if(CurrentInteractionActor.IsValid())
		CachedUIManager->InputKey();
}

void ANGPlayerController::UIClick()
{
	// NG_LOG(LogTemp, Log, TEXT("Begin"));
	CachedUIManager->InputMouseClick();
}


void ANGPlayerController::UIMouseMove(const FInputActionValue& Value)
{
	// NG_LOG(LogTemp, Log, TEXT("Begin"));
	CachedUIManager->InputMouseMove(Value);
}


void ANGPlayerController::BuildConfirm()
{
	ServerRPCConfirmPreviewActor();
	BuildingPreviewActor = nullptr;
	SetBuildMode(false);
	//SetActorTickEnabled(false);
}


void ANGPlayerController::BuildCancle()
{
	ServerRPCDestroyPreviewActor();
	BuildingPreviewActor = nullptr;

	SetBuildMode(false);
	// SetActorTickEnabled(false);
}

TObjectPtr<ANGPlayerCharacter>& ANGPlayerController::GetPlayerCharacter()
{
	if (!PlayerCharacter)
		PlayerCharacter = Cast<ANGPlayerCharacter>(GetPawn());

	return PlayerCharacter;
}
