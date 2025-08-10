// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DS1StateComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UDS1StateComponent::UDS1StateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UDS1StateComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UDS1StateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
void UDS1StateComponent::ToggleMovementInput(bool bEnabled, float Duration)
{
	if (bEnabled)
	{
		FLatentActionInfo LatentAction;
		LatentAction.CallbackTarget = this;
		LatentAction.ExecutionFunction = "MovementInputEnableAction";
		LatentAction.Linkage = 0;
		LatentAction.UUID = 0;

		UKismetSystemLibrary::RetriggerableDelay(GetWorld(), Duration, LatentAction);
	}
	else
	{
		bMovementInputEnabled = false;
	}
}
void UDS1StateComponent::MovementInputEnableAction()
{
	bMovementInputEnabled = true;
}
bool UDS1StateComponent::IsCurrentStateEqualToAny(const FGameplayTagContainer& TagsToCheck) const
{
	/*if (CurrentState == "Attacking" || CurrentState == "Rolling")
	{
		return true;
	}*/
	return TagsToCheck.HasTagExact(CurrentState);
}

