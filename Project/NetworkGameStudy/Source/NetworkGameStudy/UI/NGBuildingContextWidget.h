// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NGInputWidgetBase.h"
#include "NGBuildingContextWidget.generated.h"

class UImage;
class UTexture2D;
class UStaticMesh;

USTRUCT()
struct FDirectionHandler
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Texture;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(VisibleAnywhere)
	float MinAngle;         // 시작 각도 (inclusive)
	UPROPERTY(VisibleAnywhere)
	float MaxAngle;			// 끝 각도 (exclusive)
	UPROPERTY(VisibleAnywhere)
	float BaseAngle;			// 기준 각도
};

/**
 *
 */
UCLASS()
class NETWORKGAMESTUDY_API UNGBuildingContextWidget : public UNGInputWidgetBase
{
	GENERATED_BODY()

public:
	UNGBuildingContextWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual bool InputMouseClick() override;
	virtual bool InputMouseMove(const FInputActionValue& Value) override;
	virtual void NativeConstruct() override;
protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> HightlightImgae;

	UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> CurrentBulidingImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> BuildingCursor;


private:
	void AddOrSetDirectionRange(float MinAngle, float MaxAngle);
	const FDirectionHandler& FindDirectionHandleForAngle(float Angle);
	bool IsInRange(float Angle, float Min, float Max);

	UPROPERTY(EditAnywhere)
	TArray<FDirectionHandler> DirectionHandlers;

	FVector2D CurrentCursorPosition;

	FDirectionHandler CurrentHandle;

	const float CursorLength = 250.0f;
};
