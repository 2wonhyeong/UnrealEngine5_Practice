// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/LTFistWeapon.h"
#include "Character/LTCharacter.h"
#include "Components/LTCombatComponent.h"
#include "Components/LTWeaponCollisionComponent.h"
#include "Animation/LTAnimInstance.h"

ALTFistWeapon::ALTFistWeapon()
{
}

void ALTFistWeapon::EquipItem()
{
	CombatComponent = GetOwner()->GetComponentByClass<ULTCombatComponent>();

	if (CombatComponent)
	{
		CombatComponent->SetWeapon(this);

		if (ALTCharacter* OwnerCharacter = Cast<ALTCharacter>(GetOwner()))
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
			if (ULTAnimInstance* Anim = Cast<ULTAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()))
			{
				Anim->UpdateCombatMode(CombatType);
			}

			// ���⸦ ������ OwnerActor�� �浹���� �����մϴ�.
			WeaponCollision->AddIgnoredActor(OwnerCharacter);
			SecondWeaponCollision->AddIgnoredActor(OwnerCharacter);
		}
	}
}
