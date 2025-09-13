// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LTCombatComponent.h"
#include "Character/LTCharacter.h"
#include "Item/LTPickupItem.h"
#include "Equipments/LTWeapon.h"

// Sets default values for this component's properties
ULTCombatComponent::ULTCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULTCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULTCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULTCombatComponent::SetWeapon(ALTWeapon* NewWeapon)
{
	if (::IsValid(MainWeapon))
	{
		if (ALTCharacter* OwnerCharacter = Cast<ALTCharacter>(GetOwner()))
		{
			ALTPickupItem* PickupItem = GetWorld()->SpawnActorDeferred<ALTPickupItem>(ALTPickupItem::StaticClass(), OwnerCharacter->GetActorTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
			PickupItem->SetEquipmentClass(MainWeapon->GetClass());
			PickupItem->FinishSpawning(GetOwner()->GetActorTransform());

			MainWeapon->Destroy();
		}
	}
	MainWeapon = NewWeapon;
}

void ULTCombatComponent::SetCombatEnabled(const bool bEnabled)
{
	bCombatEnabled = bEnabled;
	if (OnChangedCombat.IsBound())
	{
		OnChangedCombat.Broadcast(bCombatEnabled);
	}
}

