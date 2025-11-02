// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/LTAttributeComponent.h"

ULTAttributeComponent::ULTAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULTAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULTAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool ULTAttributeComponent::CheckHasEnoughStamina(float StaminaCost) const
{
	return BaseStamina >= StaminaCost;
}
void ULTAttributeComponent::DecreaseStamina(float StaminaCost)
{
	//BaseStamina = FMath::Clamp(BaseStamina - StaminaCost, 0.f, MaxStamina);
	BaseStamina = BaseStamina - StaminaCost;
	if (BaseStamina < 0)
	{
		BaseStamina = 0;
	}
	if (BaseStamina > MaxStamina)
	{
		BaseStamina = MaxStamina;
	}
	BroadcastAttributeChanged(ELTAttributeType::Stamina);
}
void ULTAttributeComponent::ToggleStaminaRegeneration(bool bEnabled, float StartDelay)
{
	if (bEnabled)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(StaminaRegenTimerHandle) == false)
		{
			GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &ULTAttributeComponent::RegenStaminaHandler, 0.05f, true, StartDelay);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	}
}
void ULTAttributeComponent::BroadcastAttributeChanged(ELTAttributeType InAttributeType) const
{
	if (OnAttributeChanged.IsBound())
	{
		float Ratio = 0.f;
		switch (InAttributeType)
		{
		case ELTAttributeType::Stamina:
			Ratio = GetStaminaRatio();
			break;

		case ELTAttributeType::Health:
			Ratio = GetHealthRatio();
			break;
		}
		OnAttributeChanged.Broadcast(InAttributeType, Ratio);
	}
}
void ULTAttributeComponent::TakeDamageAmount(float DamageAmount)
{
	if (BaseHealth <= 0.f)
	{
		return;
	}
	BaseHealth = FMath::Clamp(BaseHealth - DamageAmount, 0.f, MaxHealth);
	BroadcastAttributeChanged(ELTAttributeType::Health);

	if (BaseHealth <= 0.f)
	{
		if (OnDeath.IsBound())
		{
			OnDeath.Broadcast();
		}
		if (ULTStateComponent* StateComp = GetOwner()->FindComponentByClass<ULTStateComponent>())
		{
			StateComp->SetState(LTGamePlayTags::Character_State_Death);
		}
	}
}
void ULTAttributeComponent::HealPlayer(float HealAmount)
{
	BaseHealth = FMath::Clamp(BaseHealth + HealAmount, 0.f, MaxHealth);
	BroadcastAttributeChanged(ELTAttributeType::Health);
}
void ULTAttributeComponent::RegenStaminaHandler()
{
	BaseStamina = BaseStamina + StaminaRegenRate;
	if (BaseStamina < 0)
	{
		BaseStamina = 0;
	}
	if (BaseStamina > MaxStamina)
	{
		BaseStamina = MaxStamina;
	}

	BroadcastAttributeChanged(ELTAttributeType::Stamina);

	if (BaseStamina >= MaxStamina)
	{
		ToggleStaminaRegeneration(false);
	}
}