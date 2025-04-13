// Fill out your copyright notice in the Description page of Project Settings.


#include "NGAnimNotify_NormalAttack.h"
#include "../../NetworkGameStudy.h"
#include "../NGCharacterBase.h"

void UNGAnimNotify_NormalAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// UE_LOG(LogTemp, Log, TEXT("GetNetMode : %s"), GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server"))
	ANGCharacterBase* Owner = Cast<ANGCharacterBase>(MeshComp->GetOwner());
	if (!Owner) return;

	if (Owner->GetLocalRole() == ENetRole::ROLE_Authority || Owner->GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	{
		FVector Start = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 100.f;

		Owner->ServerRPCAttack_HitCheck(Start, AttackRange);
	}
}
