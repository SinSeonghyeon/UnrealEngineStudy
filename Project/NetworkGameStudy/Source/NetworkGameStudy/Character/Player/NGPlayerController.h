// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NGPlayerController.generated.h"

class ANGPlayerCharacter;
class UUIManagerWorldSubsystem;

/**
 * 플레이어의 컨트롤러입니다.
 * 키 입력과 UI를 관리하고 있습니다.
 */
UCLASS()
class NETWORKGAMESTUDY_API ANGPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	// 임시로 스태틱 메시를 변수로 넘겨주고 있지만, 추후에는 건축 정보를 넘겨주는 식으로 변경하여야 합니다.
	void SetBuildMode(bool bIsSet, const TObjectPtr<UStaticMesh>& BuildingMesh = nullptr);

	void AddInteractionActor(TWeakObjectPtr<ANGInteractionActorBase> InteractionActor);
	void RemoveInteractionActor(TWeakObjectPtr<ANGInteractionActorBase> InteractionActor);

	UFUNCTION(Server, Reliable)
	void ServerRPCRequestDestroyActor(AActor* Actor);

private:
	UFUNCTION(Server, Reliable)
	void ServerRPCSpawnPreviewActor(UStaticMesh* BuildingMesh);

	UFUNCTION(Server, Reliable)
	void ServerRPCDestroyPreviewActor();

	UFUNCTION(Server, Reliable)
	void ServerRPCConfirmPreviewActor();

	UFUNCTION(Server, Unreliable)
	void ServerRPCMovePreviewActor(FVector Location);
protected:
	virtual void OnPossess(APawn* aPawn) override;
	
	virtual void OnUnPossess() override;

	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaTime) override;

	void SetEnhancedInput(TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	void Jump();
	void StopJumping();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void AttackStarted(const FInputActionValue& Value);
	void AttackTriggered(const FInputActionValue& Value);
	void AttackCompleted(const FInputActionValue& Value);
	void LockOn(const FInputActionValue& Value);
	void LockOnCancel(const FInputActionValue& Value);
	void ToggleBuildingWidget();
	void Interaction();
	void UIClick();
	void UIMouseMove(const FInputActionValue& Value);
	void BuildConfirm();
	void BuildCancle();

	TObjectPtr<ANGPlayerCharacter>& GetPlayerCharacter();

protected:
	TObjectPtr<ANGPlayerCharacter> PlayerCharacter;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> UIMappingContext;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> BuildMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttackAction;

	/** LockOn Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LockOnAction;

	/** LockOn Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> BuildingWidgetToggleAction;

	/** Click Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> UIClickAction;

	/** MouseMove Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> UIMouseMoveAction;

	/** BuildConfirm Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> BuildConfirmAction;

	/** BuildCancle Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> BuildCancleAction;
	
	/** IntractionAction Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IntractionAction;

private:
	TObjectPtr<UUIManagerWorldSubsystem> CachedUIManager;

	UPROPERTY(Replicated)
	TObjectPtr<ANGBuildingPreviewActor> BuildingPreviewActor;

	TSet<TWeakObjectPtr<ANGInteractionActorBase>> InteractionActorSet;
	TWeakObjectPtr<ANGInteractionActorBase> CurrentInteractionActor;

	
};
