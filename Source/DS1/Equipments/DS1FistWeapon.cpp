// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/DS1FistWeapon.h"
#include "Character/DS1Character.h"
#include "Components/DS1CombatComponent.h"
#include "Components/DS1WeaponCollisionComponent.h"
#include "Animation/DS1AnimInstance.h"

ADS1FistWeapon::ADS1FistWeapon()
{
}

void ADS1FistWeapon::EquipItem()
{
	CombatComponent = GetOwner()->GetComponentByClass<UDS1CombatComponent>();

	if (CombatComponent)
	{
		CombatComponent->SetWeapon(this);

		if (ADS1Character* OwnerCharacter = Cast<ADS1Character>(GetOwner()))
		{
			// �ָ��� ����� ����ϱ� ������ ĳ������ ���̷�Ż�޽÷� �������ش�.
			//  MainCollision
			WeaponCollision->SetWeaponMesh(OwnerCharacter->GetMesh());
			// SecondCollision
			SecondWeaponCollision->SetWeaponMesh(OwnerCharacter->GetMesh());

			// �ָ��� �����̱� ������ ���⼭ ������ �ش�.
			// ������ �����Ǵ� ������� �ִϸ��̼��� Notify�� ó��.
			CombatComponent->SetCombatEnabled(true);

			// ������ ������ CombatType���� ������Ʈ.
			if (UDS1AnimInstance* Anim = Cast<UDS1AnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()))
			{
				Anim->UpdateCombatMode(CombatType);
			}

			// ���⸦ ������ OwnerActor�� �浹���� �����մϴ�.
			WeaponCollision->AddIgnoredActor(OwnerCharacter);
			SecondWeaponCollision->AddIgnoredActor(OwnerCharacter);
		}
	}
}
