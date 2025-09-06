// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/DS1Weapon.h"

#include "DS1GamePlayTags.h"
#include "Components/DS1CombatComponent.h"
#include "Character/DS1Character.h"
#include "Components/DS1WeaponCollisionComponent.h"
#include "Animation/DS1AnimInstance.h"
#include "Data/DS1MontageActionData.h"
#include "Kismet/GameplayStatics.h"

ADS1Weapon::ADS1Weapon()
{
	WeaponCollision = CreateDefaultSubobject<UDS1WeaponCollisionComponent>("WeaponCollision");
	WeaponCollision->OnHitActor.AddUObject(this, &ADS1Weapon::OnHitActor);

	SecondWeaponCollision = CreateDefaultSubobject<UDS1WeaponCollisionComponent>("SecondCollision");
	SecondWeaponCollision->OnHitActor.AddUObject(this, &ADS1Weapon::OnHitActor);

	StaminaCostMap.Add(DS1GamePlayTags::Character_Attack_Light, 0.f);
	StaminaCostMap.Add(DS1GamePlayTags::Character_Attack_Running, 5.f);
	StaminaCostMap.Add(DS1GamePlayTags::Character_Attack_Special, 10.f);
	StaminaCostMap.Add(DS1GamePlayTags::Character_Attack_Heavy, 10.f);

	DamageMultiplierMap.Add(DS1GamePlayTags::Character_Attack_Heavy, 1.8f);
	DamageMultiplierMap.Add(DS1GamePlayTags::Character_Attack_Running, 1.8f);
	DamageMultiplierMap.Add(DS1GamePlayTags::Character_Attack_Special, 2.1f);
}

void ADS1Weapon::EquipItem()
{
	CombatComponent = GetOwner()->GetComponentByClass<UDS1CombatComponent>();

	if (CombatComponent)
	{
		CombatComponent->SetWeapon(this);

		const FName AttachSocket = CombatComponent->IsCombatEnabled() ? EquipSocketName : UnequipSocketName;

		AttachToOwner(AttachSocket);

		WeaponCollision->SetWeaponMesh(Mesh);

		if (ADS1Character* OwnerCharacter = Cast<ADS1Character>(GetOwner()))
		{
			if (UDS1AnimInstance* Anim = Cast<UDS1AnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()))
			{
				Anim->UpdateCombatMode(CombatType);
			}
		}

		WeaponCollision->AddIgnoredActor(GetOwner());
	}
}
UAnimMontage* ADS1Weapon::GetMontageForTag(const FGameplayTag& Tag, const int32 Index) const
{
	return MontageActionData->GetMontageForTag(Tag, Index);
}

float ADS1Weapon::GetStaminaCost(const FGameplayTag& InTag) const
{
	//맵에 해당 태그가 있으면
	if (StaminaCostMap.Contains(InTag))
	{
		//그 태그의 값을 리턴
		return StaminaCostMap[InTag];
	}
	return 0.f;
}

float ADS1Weapon::GetAttackDamage() const
{
	if (const AActor* OwnerActor = GetOwner())
	{
		const FGameplayTag LastAttackType = CombatComponent->GetLastAttackType();
		if (DamageMultiplierMap.Contains(LastAttackType))
		{
			const float Multiplier = DamageMultiplierMap[LastAttackType];
			return BaseDamage * Multiplier;
		}
	}
	return BaseDamage;
}

void ADS1Weapon::OnHitActor(const FHitResult& Hit)
{
	AActor* TargetActor = Hit.GetActor();

	FVector DamageDirection = GetOwner()->GetActorForwardVector();

	float AttackDamage = GetAttackDamage();

	UGameplayStatics::ApplyPointDamage(
		TargetActor,
		AttackDamage,
		DamageDirection,
		Hit,
		GetOwner()->GetInstigatorController(),
		this,
		nullptr);
}
void ADS1Weapon::ActivateCollision(EWeaponCollisionType InCollisionType)
{
	switch (InCollisionType)
	{
	case EWeaponCollisionType::MainCollision:
		WeaponCollision->TurnOnCollision();
		break;
	case EWeaponCollisionType::SecondCollision:
		SecondWeaponCollision->TurnOnCollision();
		break;
	}
}

void ADS1Weapon::DeactivateCollision(EWeaponCollisionType InCollisionType)
{
	switch (InCollisionType)
	{
	case EWeaponCollisionType::MainCollision:
		WeaponCollision->TurnOffCollision();
		break;
	case EWeaponCollisionType::SecondCollision:
		SecondWeaponCollision->TurnOffCollision();
		break;
	}
}