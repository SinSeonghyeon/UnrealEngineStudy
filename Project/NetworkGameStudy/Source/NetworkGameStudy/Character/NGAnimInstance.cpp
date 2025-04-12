// Fill out your copyright notice in the Description page of Project Settings.


#include "NGAnimInstance.h"

void UNGAnimInstance::PlayAttackMontage()
{
	if (!GetCurrentActiveMontage())
		Montage_Play(AttackMontage);
}

void UNGAnimInstance::PlayHitMontage()
{
	if(!GetCurrentActiveMontage())
		Montage_Play(HitMontage);
}
