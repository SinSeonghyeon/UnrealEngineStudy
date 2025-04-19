// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NGInteractionActorBase.generated.h"

UCLASS()
class NETWORKGAMESTUDY_API ANGInteractionActorBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANGInteractionActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnInteractionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnInteractionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	virtual FText GetInteractionText() const PURE_VIRTUAL(ANGInteractionActorBase::GetInteractionText, { return FText::FromString(TEXT("None")); };)
	virtual void DoIneraction() PURE_VIRTUAL(ANGInteractionActorBase::DoIneraction, {};)
		// Called every frame
		// virtual void Tick(float DeltaTime) override;
};
