// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Equipments/LTWeapon.h"
#include "LTCombatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FDelegateOnChangedCombat, bool);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LT_API ULTCombatComponent : public UActorComponent
{
	GENERATED_BODY()
	//���� � ���⸦ �����ϰ��ִ���
	UPROPERTY()
	ALTWeapon* MainWeapon;
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
	ULTCombatComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetWeapon(ALTWeapon* NewWeapon);

public:
	FORCEINLINE bool IsCombatEnabled() const { return bCombatEnabled; };
	void SetCombatEnabled(const bool bEnabled);
	//FORCEINLINE void SetCombatEnabled(const bool bEnabled) { bCombatEnabled = bEnabled; };

	FORCEINLINE ALTWeapon* GetMainWeapon() const { return MainWeapon; };

	FORCEINLINE FGameplayTag GetLastAttackType() const { return LastAttackType; };
	FORCEINLINE void SetLastAttackType(const FGameplayTag& NewAttackTypeTag) { LastAttackType = NewAttackTypeTag; };
		
};
