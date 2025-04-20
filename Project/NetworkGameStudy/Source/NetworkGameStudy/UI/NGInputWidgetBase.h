// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NGInputWidgetBase.generated.h"

struct FInputActionValue;

/**
 * 플레이어의 상태를 표시하는 HUD와 몬스터의 헤드업 위젯의 부모로 사용하기 위해 만든 클래스.
 * StatComponent에서 캐싱하고 있습니다.
 */
UCLASS()
class NETWORKGAMESTUDY_API UNGInputWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
//public:
//	UNGInputWidgetBase(const FObjectInitializer& ObjectInitializer);

public:
	// -------------------- true를 반환하면 사용됨 -----------------------------
	// 해당 인풋이 사용되었는지 여부를 반환합니다. 다른 위젯에서 중복 사용을 막기 위함.
	virtual bool InputMouseClick();
	virtual bool InputMouseMove(const FInputActionValue& Value);
	virtual bool InputKey();
	virtual bool InputKeyRelease();
protected:
	virtual void NativeConstruct() override;

	void SetInputWidgetBaseParent(const TObjectPtr<UNGInputWidgetBase> Parent);

private:
	void SetInputWidgetBaseChild(const TObjectPtr<UNGInputWidgetBase> Child);

protected:
	UPROPERTY(Transient)
	TArray<TObjectPtr<UNGInputWidgetBase>> UserWidgetChildren;
};
