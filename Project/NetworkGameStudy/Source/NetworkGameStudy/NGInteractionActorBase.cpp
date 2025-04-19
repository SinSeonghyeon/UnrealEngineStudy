// Fill out your copyright notice in the Description page of Project Settings.


#include "NGInteractionActorBase.h"


// Sets default values
ANGInteractionActorBase::ANGInteractionActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANGInteractionActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANGInteractionActorBase::OnInteractionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ANGPlayerCharacter* Character = Cast<ANGPlayerCharacter>(OtherActor))
	{
		if (ANGPlayerController* Controller = Cast<ANGPlayerController>(Character->GetController()))
		{
			Controller->AddInteractionActor(this);
		}
	}
}

void ANGInteractionActorBase::OnInteractionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ANGPlayerCharacter* Character = Cast<ANGPlayerCharacter>(OtherActor))
	{
		if (ANGPlayerController* Controller = Cast<ANGPlayerController>(Character->GetController()))
		{
			Controller->RemoveInteractionActor(this);
		}
	}
}

// Called every frame
//void ANGInteractionActorBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}
//
