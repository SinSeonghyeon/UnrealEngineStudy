// Fill out your copyright notice in the Description page of Project Settings.


#include "NGBuildingContextWidget.h"
#include "NetworkGameStudy.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/CanvasPanelSlot.h"

UNGBuildingContextWidget::UNGBuildingContextWidget(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	CurrentCursorPosition.Y = -CursorLength;

	const float SegmentSize = 60.0f; // 360 / 5

	for (int32 i = 0; i < 6; ++i)
	{
		float Start = -180.f + (i * SegmentSize);
		float End = Start + SegmentSize;

		AddOrSetDirectionRange(Start, End);
	}
}

bool UNGBuildingContextWidget::InputMouseClick()
{
	if (Super::InputMouseClick())
		return true;

	if (CurrentHandle.Mesh)
		Cast<ANGPlayerController>(GetWorld()->GetFirstPlayerController())->SetBuildMode(true, CurrentHandle.Mesh);

	// 컨트롤러에 이벤트를 던줘야합니다.
	return true;
}


bool UNGBuildingContextWidget::InputMouseMove(const FInputActionValue& Value)
{
	if (Super::InputMouseMove(Value))
		return true;

	// ------------ 커서 움직임 --------------------------
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	UE_LOG(LogTemp, Log, TEXT("MouseMove x : %lf y : %lf"), LookAxisVector.X, LookAxisVector.Y);

	LookAxisVector.Y = -LookAxisVector.Y;
	FVector2D NormalizedCursorPos = CurrentCursorPosition + LookAxisVector * 5.0f;
	NormalizedCursorPos.Normalize();

	CurrentCursorPosition = NormalizedCursorPos * CursorLength;

	FWidgetTransform Transform;
	Transform.Translation = CurrentCursorPosition;

	BuildingCursor->SetRenderTransform(Transform);

	// ------------ 하이라이트 활성화 --------------------------
	float Angle = FMath::Atan2(NormalizedCursorPos.X, NormalizedCursorPos.Y);
	UE_LOG(LogTemp, Log, TEXT("CursorAngle : %f"), Angle);

	float DegreeAngle = FMath::RadiansToDegrees(Angle);

	CurrentHandle = FindDirectionHandleForAngle(DegreeAngle);

	HightlightImgae->SetRenderTransformAngle(-CurrentHandle.BaseAngle);

	// ----------------------- 현재 선택된 건축물을 보여줍니다. -----------------------
	if (CurrentHandle.Texture)
	{
		CurrentBulidingImage->SetBrushFromTexture(CurrentHandle.Texture);
		CurrentBulidingImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CurrentBulidingImage->SetVisibility(ESlateVisibility::Collapsed);
	}

	return true;
}


void UNGBuildingContextWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (TObjectPtr<UPanelWidget> Panel = Cast<UPanelWidget>(GetRootWidget()))
	{
		for (int i = 0; i < DirectionHandlers.Num(); ++i)
		{
			if (DirectionHandlers[i].Texture)
			{
				TObjectPtr<UImage> Child = NewObject<UImage>(this);
				Panel->AddChild(Child);
				Child->SetBrushFromTexture(DirectionHandlers[i].Texture);
				TObjectPtr<UCanvasPanelSlot> ChildSlot = Cast<UCanvasPanelSlot>(Child->Slot);
				if (ChildSlot)
				{
					// 1. 앵커 설정 (예: 가운데 정렬)
					ChildSlot->SetAnchors(FAnchors(0.5f, 0.5f));

					// 2. 정렬 (0.5, 0.5 == 중앙 기준)
					ChildSlot->SetAlignment(FVector2D(0.5f, 0.5f));

					float Radian = FMath::DegreesToRadians(DirectionHandlers[i].BaseAngle - 90.0f);
					// 3. 위치 설정 (앵커를 기준으로 한 Offset)
					ChildSlot->SetPosition(FVector2D(FMath::Cos(Radian) * 300, FMath::Sin(Radian) * 300));

					// 4. 크기 설정
					ChildSlot->SetSize(FVector2D(300.f, 300.f));
				}
			}
			else
			{
				break;
			}
		}
	}

	FInputActionValue Value;
	InputMouseMove(Value);
}


void UNGBuildingContextWidget::AddOrSetDirectionRange(float MinAngle, float MaxAngle)
{
	DirectionHandlers.Add({ nullptr, nullptr, MinAngle, MaxAngle, (MinAngle + MaxAngle) / 2.0f });
}

const FDirectionHandler& UNGBuildingContextWidget::FindDirectionHandleForAngle(float Angle)
{
	float NormalizedAngle = FMath::UnwindDegrees(Angle); // -180 ~ 180 범위로 보정

	for (const FDirectionHandler& Handler : DirectionHandlers)
	{
		if (IsInRange(NormalizedAngle, Handler.MinAngle, Handler.MaxAngle))
		{
			return Handler;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("No handler for angle: %.2f"), Angle);

	return DirectionHandlers[0];
}

bool UNGBuildingContextWidget::IsInRange(float Angle, float Min, float Max)
{
	return (Angle >= Min && Angle < Max);
}
