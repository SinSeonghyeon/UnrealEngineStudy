// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NGStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHealthZero);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NETWORKGAMESTUDY_API UNGStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UNGStatComponent();

public:
	void SetMaxHP(int32 InHP) { MaxHP = InHP; }
	int32 GetMaxHP() const { return MaxHP; }

	void SetCurrentHP(int32 InHP) { CurrentHP = InHP; }
	int32 GetCurrentHP() const { return CurrentHP; }

	void SetDamage(int32 InDamage) { Damage = InDamage; }
	int32 GetDamage() const { return Damage; }

	// 서버에서 호출됩니다.
	void TakeDamage(int32 InDamage);

	void Initialize(const TObjectPtr<UNGStatWidgetBase>& InStatWidget);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

private:
	UFUNCTION()
	void OnRep_ChangeMaxHP();

	UFUNCTION()
	void OnRep_ChangeCurrentHP();

public:
	// 체력이 0이하로 떨어지면 호출되는 델리게이트입니다.
	FOnHealthZero OnHealthZero;
private:
	UPROPERTY(ReplicatedUsing = OnRep_ChangeMaxHP, VisibleAnywhere, Category = Stat, meta = (AllowPrivateAccess = "true"))
	int32 MaxHP;
	UPROPERTY(ReplicatedUsing = OnRep_ChangeCurrentHP, VisibleAnywhere, Category = Stat, meta = (AllowPrivateAccess = "true"))
	int32 CurrentHP;
	UPROPERTY(VisibleAnywhere, Category = Stat, meta = (AllowPrivateAccess = "true"))
	int32 Damage;

	TObjectPtr<UNGStatWidgetBase> MyStatWidget;
};
