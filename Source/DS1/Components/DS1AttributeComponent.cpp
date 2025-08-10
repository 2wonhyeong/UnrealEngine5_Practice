// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/DS1AttributeComponent.h"

UDS1AttributeComponent::UDS1AttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDS1AttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDS1AttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UDS1AttributeComponent::CheckHasEnoughStamina(float StaminaCost) const
{
	return BaseStamina >= StaminaCost;
}
void UDS1AttributeComponent::DecreaseStamina(float StaminaCost)
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
	BroadcastAttributeChanged(EDS1AttributeType::Stamina);
}
void UDS1AttributeComponent::ToggleStaminaRegeneration(bool bEnabled, float StartDelay)
{
	if (bEnabled)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(StaminaRegenTimerHandle) == false)
		{
			GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &UDS1AttributeComponent::RegenStaminaHandler, 0.1f, true, StartDelay);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	}
}
void UDS1AttributeComponent::BroadcastAttributeChanged(EDS1AttributeType InAttributeType) const
{
	if (OnAttributeChanged.IsBound())
	{
		float Ratio = 0.f;
		switch (InAttributeType)
		{
		case EDS1AttributeType::Stamina:
			Ratio = GetStaminaRatio();
			break;

		case EDS1AttributeType::Health:
			break;
		}
		OnAttributeChanged.Broadcast(InAttributeType, Ratio);
	}
}
void UDS1AttributeComponent::RegenStaminaHandler()
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

	BroadcastAttributeChanged(EDS1AttributeType::Stamina);

	if (BaseStamina >= MaxStamina)
	{
		ToggleStaminaRegeneration(false);
	}
}