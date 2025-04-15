// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NGPlayerController.generated.h"

/**
 * 키 입력도 컨트롤러 클래스로 옮기는 것이 좋을 것 같지만,,
 * 시간이 없으니 우선 UI만 붙입니다.
 */
UCLASS()
class NETWORKGAMESTUDY_API ANGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
protected:
	virtual void BeginPlay() override;
	
};
