// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Equipments/DS1Weapon.h"
#include "DS1CombatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FDelegateOnChangedCombat, bool);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DS1_API UDS1CombatComponent : public UActorComponent
{
	GENERATED_BODY()
	//현재 어떤 무기를 장착하고있는지
	UPROPERTY()
	ADS1Weapon* MainWeapon;
	//전투 활성화 상태인지
	UPROPERTY(EditAnywhere)
	bool bCombatEnabled = false;
	//마지막 공격의 타입
	UPROPERTY(VisibleAnywhere)
	FGameplayTag LastAttackType;

public:
	//Combat상태 전환을 알리는 델리게이트
	FDelegateOnChangedCombat OnChangedCombat;

public:	
	UDS1CombatComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetWeapon(ADS1Weapon* NewWeapon);

public:
	FORCEINLINE bool IsCombatEnabled() const { return bCombatEnabled; };
	void SetCombatEnabled(const bool bEnabled);
	//FORCEINLINE void SetCombatEnabled(const bool bEnabled) { bCombatEnabled = bEnabled; };

	FORCEINLINE ADS1Weapon* GetMainWeapon() const { return MainWeapon; };

	FORCEINLINE FGameplayTag GetLastAttackType() const { return LastAttackType; };
	FORCEINLINE void SetLastAttackType(const FGameplayTag& NewAttackTypeTag) { LastAttackType = NewAttackTypeTag; };
		
};
