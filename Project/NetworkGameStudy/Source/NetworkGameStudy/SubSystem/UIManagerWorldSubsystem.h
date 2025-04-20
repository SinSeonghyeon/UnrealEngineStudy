// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "UIManagerWorldSubsystem.generated.h"

class UNGMainPanelWidget;

/**
 *
 */
UCLASS()
class NETWORKGAMESTUDY_API UUIManagerWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UUIManagerWorldSubsystem();

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void ShowMainUI(APlayerController* PlayerController);
	void HideMainUI();

	bool ToggleBuildingContextUI();
	bool ToggleInventoryUI();

	void InputMouseClick();
	void InputKey();
	void InputKeyRelease();
	void InputMouseMove(const FInputActionValue& Value);

	void SetInteractionWidget(ANGInteractionActorBase* InteractionActor);

	void UpdateItemWidget(const TMap<FName, int32>& ItemList);

	const TObjectPtr<UNGMainPanelWidget>& GeMainWidgetInstance() { return MainUIInstance; }
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UNGMainPanelWidget> MainUIClass;

	TObjectPtr<UNGMainPanelWidget> MainUIInstance;
};
