// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NGInputWidgetBase.h"
#include "NGStatWidgetBase.generated.h"

/**
 * 플레이어의 상태를 표시하는 HUD와 몬스터의 헤드업 위젯의 부모로 사용하기 위해 만든 클래스.
 * StatComponent에서 캐싱하고 있습니다.
 */
UCLASS()
class NETWORKGAMESTUDY_API UNGStatWidgetBase : public UNGInputWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void UpdateHPProgress(float Percent) {};
	virtual void UpdateMaxHP(int32 HP) {};
	virtual void UpdateCurrentHP(int32 HP) {};
};
