// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NGInteractionActorBase.h"
#include "NGBuildingPreviewActor.generated.h"

class UStaticMeshComponent;
class UStaticMesh;

/**
 *
 */
UCLASS()
class NETWORKGAMESTUDY_API ANGBuildingPreviewActor : public ANGInteractionActorBase
{
	GENERATED_BODY()

public:
	ANGBuildingPreviewActor();

public:
	// 서버에서 호출합니다.
	void SetBuildingMesh(UStaticMesh* NewMesh);

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual FText GetInteractionText() const { return FText::FromString(TEXT("건축")); }

	virtual void DoIneraction() override;
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_ChangedMesh();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BuildingMeshComponent;

	UPROPERTY(ReplicatedUsing = OnRep_ChangedMesh)
	UStaticMesh* BuildingMesh;

	TObjectPtr<UMaterialInstanceDynamic> DynMaterial;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialInterface> BaseMaterial;

	float Percent = 0.0f;


};
