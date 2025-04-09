// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NGDropItemActor.generated.h"

UCLASS()
class NETWORKGAMESTUDY_API ANGDropItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANGDropItemActor();

public:
	void Initialize(FName InItemID);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_MeshChanged)
	UStaticMesh* ReplicatedMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    FName ItemID;

private:
	UFUNCTION()
	void OnRep_MeshChanged();
};
