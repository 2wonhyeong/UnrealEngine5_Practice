// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LTStateComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ULTStateComponent::ULTStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void ULTStateComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void ULTStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
void ULTStateComponent::ToggleMovementInput(bool bEnabled, float Duration)
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
void ULTStateComponent::MovementInputEnableAction()
{
	bMovementInputEnabled = true;
	ClearState();
}
void ULTStateComponent::ClearState()
{
	if (CurrentState == LTGamePlayTags::Character_State_Death)
		return;
	CurrentState = FGameplayTag::EmptyTag;
}
bool ULTStateComponent::IsCurrentStateEqualToAny(const FGameplayTagContainer& TagsToCheck) const
{
	/*if (CurrentState == "Attacking" || CurrentState == "Rolling")
	{
		return true;
	}*/
	return TagsToCheck.HasTagExact(CurrentState);
}

