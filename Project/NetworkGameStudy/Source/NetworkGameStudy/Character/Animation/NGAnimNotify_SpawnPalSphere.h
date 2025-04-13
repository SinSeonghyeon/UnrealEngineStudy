// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "NGAnimNotify_SpawnPalSphere.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKGAMESTUDY_API UNGAnimNotify_SpawnPalSphere : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
