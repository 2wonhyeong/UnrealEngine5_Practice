// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LTPotionInventoryComponent.h"
#include "Components/LTAttributeComponent.h"
#include "GameFramework/Character.h"
#include "Item/LTPotion.h"

// Sets default values for this component's properties
ULTPotionInventoryComponent::ULTPotionInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULTPotionInventoryComponent::DrinkPotion()
{
	if (PotionQuantity <= 0)
		return;
	
	if (ULTAttributeComponent* Attribute = GetOwner()->GetComponentByClass< ULTAttributeComponent>())
	{
		PotionQuantity--;
		Attribute->HealPlayer(PotionHealAmount);
		BroadcastPotionUpdate();
	}
}

void ULTPotionInventoryComponent::SpawnPotion()
{
	PotionActor = GetWorld()->SpawnActor<ALTPotion>(PotionClass, GetOwner()->GetActorTransform());
	if (PotionActor)
	{
		if (const ACharacter* Character = Cast<ACharacter>(GetOwner()))
		{
			PotionActor->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), PotionSocketName);
		}
	}
}

void ULTPotionInventoryComponent::DespawnPotion()
{
	if (IsValid(PotionActor))
	{
		PotionActor->Destroy();
	}
}

void ULTPotionInventoryComponent::SetPotionQuantity(int32 InQuantity)
{
	PotionQuantity = InQuantity;
	BroadcastPotionUpdate();
}

void ULTPotionInventoryComponent::BroadcastPotionUpdate() const
{
	if (OnUpdatePotionAmount.IsBound())
	{
		OnUpdatePotionAmount.Broadcast(PotionQuantity);
	}
}

