// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NGInteractionActorBase.h"
#include "NGDropItemActor.generated.h"

UCLASS()
class NETWORKGAMESTUDY_API ANGDropItemActor : public ANGInteractionActorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANGDropItemActor();

public:
	virtual FText GetInteractionText() const { return FText::FromString(TEXT("획득")); }
	virtual void DoIneraction(ANGPlayerController* PlayerController) override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_MeshChanged)
	TObjectPtr<UStaticMesh> Mesh_Rep;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    FName ItemID;

private:
	UFUNCTION()
	void OnRep_MeshChanged();
};
