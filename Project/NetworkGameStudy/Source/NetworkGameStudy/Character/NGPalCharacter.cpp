// Copyright Epic Games, Inc. All Rights Reserved.

#include "NGPalCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


//////////////////////////////////////////////////////////////////////////
// ANGPalCharacter

ANGPalCharacter::ANGPalCharacter()
{
	bUseControllerRotationYaw = true;
}

void ANGPalCharacter::UpdateMaxWalkSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}
