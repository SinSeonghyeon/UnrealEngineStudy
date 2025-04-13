// Fill out your copyright notice in the Description page of Project Settings.


#include "NGAnimNotify_ChangeWeaponMesh.h"
#include "../NGPlayerCharacter.h"

void UNGAnimNotify_ChangeWeaponMesh::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// UE_LOG(LogTemp, Log, TEXT("GetNetMode : %s"), GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server"))
	ANGPlayerCharacter* Owner = Cast<ANGPlayerCharacter>(MeshComp->GetOwner());
	if (!Owner) return;

	if (Owner->GetLocalRole() == ENetRole::ROLE_Authority || Owner->GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	{
		Owner->ServerRPCChangeWeaponMesh(WeaponName);
	}
}
