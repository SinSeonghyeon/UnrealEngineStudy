// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NGCharacterBase.h"
#include "NGPalCharacter.generated.h"

UCLASS(config=Game)
class ANGPalCharacter : public ANGCharacterBase
{
	GENERATED_BODY()
public:
	ANGPalCharacter();

public:    
    UFUNCTION(BlueprintCallable, Category = "NGCustom")
    void UpdateMaxWalkSpeed(float NewSpeed);

	TObjectPtr<ANGCharacterBase> GetPalOwner() const { return PalOwner; };

private:
	TObjectPtr<ANGCharacterBase> PalOwner;

};

