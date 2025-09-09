// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/DS1Character.h"
#include "DS1Define.h"
#include "Equipments/DS1Equipment.h"
#include "DS1Weapon.generated.h"

class UDS1WeaponCollisionComponent;
class UDS1MontageActionData;
struct FGameplayTag;
class DS1CombatComponent;
/**
 * 
 */
UCLASS()
class DS1_API ADS1Weapon : public ADS1Equipment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	FName EquipSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	FName UnequipSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	UDS1MontageActionData* MontageActionData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	ECombatType CombatType = ECombatType::SwordShield;

protected:
	UPROPERTY(VisibleAnywhere)
	UDS1WeaponCollisionComponent* WeaponCollision;
	UPROPERTY(VisibleAnywhere)
	UDS1WeaponCollisionComponent* SecondWeaponCollision;

protected:
	UPROPERTY()
	UDS1CombatComponent* CombatComponent;
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float>StaminaCostMap;
	UPROPERTY(EditAnywhere)
	float BaseDamage = 15.f;
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> DamageMultiplierMap;

public:
	ADS1Weapon();
	
public:
	virtual void EquipItem() override;

	UAnimMontage* GetMontageForTag(const FGameplayTag& Tag, const int32 Index = 0) const;

	float GetStaminaCost(const FGameplayTag& InTag) const;
	float GetAttackDamage() const;

	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; };
	FORCEINLINE FName GetUnequipSocketName() const { return UnequipSocketName; };
	FORCEINLINE UDS1WeaponCollisionComponent* GetCollision() const { return WeaponCollision; };
	FORCEINLINE ECombatType GetCombatType() const { return CombatType; };

public:
	virtual void ActivateCollision(EWeaponCollisionType InCollisionType);
	virtual void DeactivateCollision(EWeaponCollisionType InCollisionType);

public:
	void OnHitActor(const FHitResult& Hit);
};
