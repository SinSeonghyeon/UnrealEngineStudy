// Copyright Epic Games, Inc. All Rights Reserved.

#include "NGPalCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISense_Damage.h"


//////////////////////////////////////////////////////////////////////////
// ANGPalCharacter

ANGPalCharacter::ANGPalCharacter()
{
	bUseControllerRotationYaw = true;
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

void ANGPalCharacter::UpdateMaxWalkSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

