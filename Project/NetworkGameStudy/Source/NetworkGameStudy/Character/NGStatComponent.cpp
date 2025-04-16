// Fill out your copyright notice in the Description page of Project Settings.


#include "NGStatComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UNGStatComponent::UNGStatComponent()
{
	MaxHP = 1000.0f;
	CurrentHP = MaxHP;
	Damage = 100.0f;

	SetIsReplicatedByDefault(true);
}

void UNGStatComponent::TakeDamage(int32 InDamage)
{
	CurrentHP -= InDamage;

	if (CurrentHP <= 0)
	{
		OnHealthZero.Broadcast();
	}
}

void UNGStatComponent::Initialize(const TObjectPtr<UNGStatWidgetBase>& InStatWidget)
{
	MyStatWidget = InStatWidget;

	if (GetNetMode() == ENetMode::NM_Client)
	{
		OnRep_ChangeMaxHP();
		OnRep_ChangeCurrentHP();
	}
}

// Called when the game starts
void UNGStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UNGStatComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UNGStatComponent, MaxHP);
	DOREPLIFETIME(UNGStatComponent, CurrentHP);
}

void UNGStatComponent::OnRep_ChangeMaxHP()
{
	if (MyStatWidget)
		MyStatWidget->UpdateMaxHP(MaxHP);
	// UI update 필요
}

void UNGStatComponent::OnRep_ChangeCurrentHP()
{
	if (MyStatWidget)
	{
		MyStatWidget->UpdateCurrentHP(CurrentHP);
		MyStatWidget->UpdateHPProgress(static_cast<float>(CurrentHP) / static_cast<float>(MaxHP));
	}
	// UI update 필요
}