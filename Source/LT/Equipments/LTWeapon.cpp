// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/LTWeapon.h"

#include "LTGamePlayTags.h"
#include "Components/LTCombatComponent.h"
#include "Character/LTCharacter.h"
#include "Components/LTWeaponCollisionComponent.h"
#include "Animation/LTAnimInstance.h"
#include "Data/LTMontageActionData.h"
#include "Kismet/GameplayStatics.h"

ALTWeapon::ALTWeapon()
{
	WeaponCollision = CreateDefaultSubobject<ULTWeaponCollisionComponent>("WeaponCollision");
	WeaponCollision->OnHitActor.AddUObject(this, &ALTWeapon::OnHitActor);

	SecondWeaponCollision = CreateDefaultSubobject<ULTWeaponCollisionComponent>("SecondCollision");
	SecondWeaponCollision->OnHitActor.AddUObject(this, &ALTWeapon::OnHitActor);

	StaminaCostMap.Add(LTGamePlayTags::Character_Attack_Light, 0.f);
	StaminaCostMap.Add(LTGamePlayTags::Character_Attack_Running, 5.f);
	StaminaCostMap.Add(LTGamePlayTags::Character_Attack_Special, 10.f);
	StaminaCostMap.Add(LTGamePlayTags::Character_Attack_Heavy, 10.f);

	DamageMultiplierMap.Add(LTGamePlayTags::Character_Attack_Heavy, 1.8f);
	DamageMultiplierMap.Add(LTGamePlayTags::Character_Attack_Running, 1.8f);
	DamageMultiplierMap.Add(LTGamePlayTags::Character_Attack_Special, 2.1f);
}

void ALTWeapon::EquipItem()
{
	CombatComponent = GetOwner()->GetComponentByClass<ULTCombatComponent>();

	if (CombatComponent)
	{
		CombatComponent->SetWeapon(this);

		const FName AttachSocket = CombatComponent->IsCombatEnabled() ? EquipSocketName : UnequipSocketName;

		AttachToOwner(AttachSocket);

		WeaponCollision->SetWeaponMesh(Mesh);

		if (ALTCharacter* OwnerCharacter = Cast<ALTCharacter>(GetOwner()))
		{
			if (ULTAnimInstance* Anim = Cast<ULTAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()))
			{
				Anim->UpdateCombatMode(CombatType);
			}
		}

		WeaponCollision->AddIgnoredActor(GetOwner());
	}
}
UAnimMontage* ALTWeapon::GetMontageForTag(const FGameplayTag& Tag, const int32 Index) const
{
	return MontageActionData->GetMontageForTag(Tag, Index);
}

float ALTWeapon::GetStaminaCost(const FGameplayTag& InTag) const
{
	//맵에 해당 태그가 있으면
	if (StaminaCostMap.Contains(InTag))
	{
		//그 태그의 값을 리턴
		return StaminaCostMap[InTag];
	}
	return 0.f;
}

float ALTWeapon::GetAttackDamage() const
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

void ALTWeapon::OnHitActor(const FHitResult& Hit)
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
void ALTWeapon::ActivateCollision(EWeaponCollisionType InCollisionType)
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

void ALTWeapon::DeactivateCollision(EWeaponCollisionType InCollisionType)
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