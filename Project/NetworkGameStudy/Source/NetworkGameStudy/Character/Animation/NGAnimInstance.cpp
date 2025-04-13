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

void UNGAnimInstance::PlayLockOn()
{
	if (!GetCurrentActiveMontage())
		Montage_Play(OnLockMontage);
}

void UNGAnimInstance::PlayThrow()
{
	if (Montage_IsPlaying(OnLockMontage))
	{
		Montage_Play(OnLockMontage);
		Montage_JumpToSection(TEXT("Throw"), OnLockMontage);
		//Montage_SetNextSection(TEXT("Loop"), TEXT("Throw"), OnLockMontage);

		//Montage_SetPosition(OnLockMontage, 0);
	}
}

void UNGAnimInstance::StopLockOn()
{
	if (Montage_IsPlaying(OnLockMontage))
	{
		Montage_Stop(0.25, OnLockMontage);
	}
}

void UNGAnimInstance::PlayAnimFromName(FName AnimName)
{
	if (AnimName == TEXT("Attack"))
	{
		PlayAttackMontage();
	}
	else if (AnimName == TEXT("Hit"))
	{
		PlayHitMontage();
	}
	else if (AnimName == TEXT("LockOn"))
	{
		PlayLockOn();
	}
	else if (AnimName == TEXT("Throw"))
	{
		PlayThrow();
	}
	else if (AnimName == TEXT("StopLockOn"))
	{
		StopLockOn();
	}
}
