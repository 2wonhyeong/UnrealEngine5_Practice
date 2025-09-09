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
	//���� � ���⸦ �����ϰ��ִ���
	UPROPERTY()
	ADS1Weapon* MainWeapon;
	//���� Ȱ��ȭ ��������
	UPROPERTY(EditAnywhere)
	bool bCombatEnabled = false;
	//������ ������ Ÿ��
	UPROPERTY(VisibleAnywhere)
	FGameplayTag LastAttackType;

public:
	//Combat���� ��ȯ�� �˸��� ��������Ʈ
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
