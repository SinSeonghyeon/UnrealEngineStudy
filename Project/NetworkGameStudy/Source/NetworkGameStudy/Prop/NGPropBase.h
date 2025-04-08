// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NGPropBase.generated.h"

UCLASS()
class NETWORKGAMESTUDY_API ANGPropBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANGPropBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
