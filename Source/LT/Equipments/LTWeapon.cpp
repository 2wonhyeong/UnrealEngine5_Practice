// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/LTWeapon.h"

#include "LTGamePlayTags.h"
#include "Components/LTCombatComponent.h"
#include "Character/LTCharacter.h"
#include "Components/LTWeaponCollisionComponent.h"
#include "Animation/LTAnimInstance.h"
#include "Data/LTMontageActionData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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

		if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
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
UAnimMontage* ALTWeapon::GetRandomMontageForTag(const FGameplayTag& Tag) const
{
	return MontageActionData->GetRandomMontageForTag(Tag);
}
UAnimMontage* ALTWeapon::GetHitReactMontage(const AActor* Attacker) const
{
	// LookAt 회전값을 구합니다. (현재 Actor가 공격자를 바라보는 회전값)
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Attacker->GetActorLocation());
	// 현재 Actor의 회전값과 LookAt 회전값의 차이를 구합니다.
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), LookAtRotation);
	// Z축 기준의 회전값 차이만을 취합니다.
	const float DeltaZ = DeltaRotation.Yaw;

	EHitDirection HitDirection = EHitDirection::Front;

	if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -45.f, 45.f))
	{
		HitDirection = EHitDirection::Front;
		UE_LOG(LogTemp, Log, TEXT("Front"));
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 45.f, 135.f))
	{
		HitDirection = EHitDirection::Left;
		UE_LOG(LogTemp, Log, TEXT("Left"));
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 135.f, 180.f)
		|| UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -180.f, -135.f))
	{
		HitDirection = EHitDirection::Back;
		UE_LOG(LogTemp, Log, TEXT("Back"));
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -135.f, -45.f))
	{
		HitDirection = EHitDirection::Right;
		UE_LOG(LogTemp, Log, TEXT("Right"));
	}

	UAnimMontage* SelectedMontage = nullptr;
	switch (HitDirection)
	{
	case EHitDirection::Front:
		SelectedMontage = GetMontageForTag(LTGamePlayTags::Character_Action_HitReaction, 0);
		break;
	case EHitDirection::Back:
		SelectedMontage = GetMontageForTag(LTGamePlayTags::Character_Action_HitReaction, 1);
		break;
	case EHitDirection::Left:
		SelectedMontage = GetMontageForTag(LTGamePlayTags::Character_Action_HitReaction, 2);
		break;
	case EHitDirection::Right:
		SelectedMontage = GetMontageForTag(LTGamePlayTags::Character_Action_HitReaction, 3);
		break;
	}

	return SelectedMontage;
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