// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/LTCharacter.h"
#include "LTDefine.h"
#include "Equipments/LTEquipment.h"
#include "LTWeapon.generated.h"

class ULTWeaponCollisionComponent;
class ULTMontageActionData;
struct FGameplayTag;
class ULTCombatComponent;
/**
 * 
 */
UCLASS()
class LT_API ALTWeapon : public ALTEquipment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	FName EquipSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	FName UnequipSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	ULTMontageActionData* MontageActionData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	ECombatType CombatType = ECombatType::SwordShield;

protected:
	UPROPERTY(VisibleAnywhere)
	ULTWeaponCollisionComponent* WeaponCollision;
	UPROPERTY(VisibleAnywhere)
	ULTWeaponCollisionComponent* SecondWeaponCollision;

protected:
	UPROPERTY()
	ULTCombatComponent* CombatComponent;
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float>StaminaCostMap;
	UPROPERTY(EditAnywhere)
	float BaseDamage = 15.f;
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> DamageMultiplierMap;

public:
	ALTWeapon();
	
public:
	virtual void EquipItem() override;

	UAnimMontage* GetMontageForTag(const FGameplayTag& Tag, const int32 Index = 0) const;
	UAnimMontage* GetRandomMontageForTag(const FGameplayTag& Tag) const;

	float GetStaminaCost(const FGameplayTag& InTag) const;
	float GetAttackDamage() const;

	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; };
	FORCEINLINE FName GetUnequipSocketName() const { return UnequipSocketName; };
	FORCEINLINE ULTWeaponCollisionComponent* GetCollision() const { return WeaponCollision; };
	FORCEINLINE ECombatType GetCombatType() const { return CombatType; };

public:
	virtual void ActivateCollision(EWeaponCollisionType InCollisionType);
	virtual void DeactivateCollision(EWeaponCollisionType InCollisionType);

public:
	void OnHitActor(const FHitResult& Hit);
};
