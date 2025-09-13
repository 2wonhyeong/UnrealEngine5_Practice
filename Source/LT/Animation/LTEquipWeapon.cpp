// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/LTEquipWeapon.h"

#include "LTGamePlayTags.h"
#include "Components/LTCombatComponent.h"
#include "Equipments/LTWeapon.h"

ULTEquipWeapon::ULTEquipWeapon(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void ULTEquipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (const AActor* Owner = MeshComp->GetOwner())
	{
		if (ULTCombatComponent* CombatComponent = Owner->GetComponentByClass<ULTCombatComponent>())
		{
			if (ALTWeapon* MainWeapon = CombatComponent->GetMainWeapon())
			{
				bool bCombatEnabled = CombatComponent->IsCombatEnabled();
				FName WeaponSocketName;

				if (MontageActionTag == LTGamePlayTags::Character_Action_Equip)
				{
					bCombatEnabled = true;
					WeaponSocketName = MainWeapon->GetEquipSocketName();
				}
				else if (MontageActionTag == LTGamePlayTags::Character_Action_Unequip)
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
