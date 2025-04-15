// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "UIManagerWorldSubsystem.generated.h"

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

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowMainUI(APlayerController* PlayerController);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void HideMainUI();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> MainUIClass;

    UPROPERTY()
    UUserWidget* MainUIInstance;
	
	
};
