// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DS1EquipWeapon.h"

#include "DS1GamePlayTags.h"
#include "Components/DS1CombatComponent.h"
#include "Equipments/DS1Weapon.h"

UDS1EquipWeapon::UDS1EquipWeapon(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UDS1EquipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (const AActor* Owner = MeshComp->GetOwner())
	{
		if (UDS1CombatComponent* CombatComponent = Owner->GetComponentByClass<UDS1CombatComponent>())
		{
			if (ADS1Weapon* MainWeapon = CombatComponent->GetMainWeapon())
			{
				bool bCombatEnabled = CombatComponent->IsCombatEnabled();
				FName WeaponSocketName;

				if (MontageActionTag == DS1GamePlayTags::Character_Action_Equip)
				{
					bCombatEnabled = true;
					WeaponSocketName = MainWeapon->GetEquipSocketName();
				}
				else if (MontageActionTag == DS1GamePlayTags::Character_Action_Unequip)
				{
					bCombatEnabled = false;
					WeaponSocketName = MainWeapon->GetUnequipSocketName();
				}

				// AttachToPlayer 함수보다 먼저 호출해야한다.
				CombatComponent->SetCombatEnabled(bCombatEnabled);
				MainWeapon->AttachToOwner(WeaponSocketName);
			}
		}
	}
}
