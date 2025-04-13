// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "NGAnimNotify_ChangeWeaponMesh.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKGAMESTUDY_API UNGAnimNotify_ChangeWeaponMesh : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	// 반지름 값을 입력해주세요.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	FName WeaponName = TEXT("PalSphere");

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
