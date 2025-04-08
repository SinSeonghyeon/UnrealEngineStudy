// Fill out your copyright notice in the Description page of Project Settings.


#include "NGAnimNotify_NormalAttack.h"
#include "../NetworkGameStudy.h"
#include "../NetworkGameStudyCharacter.h"

void UNGAnimNotify_NormalAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ANetworkGameStudyCharacter* Owner = Cast<ANetworkGameStudyCharacter>(MeshComp->GetOwner());
	if (!Owner) return;

	FVector Start = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 100.f;

	Owner->ServerRPCAttack_HitCheck(Start, AttackRange);
}
