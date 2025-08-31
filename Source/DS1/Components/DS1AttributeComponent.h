// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DS1Define.h"
#include "Components/DS1StateComponent.h"
#include "DS1GamePlayTags.h"
#include "DS1AttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FDelegateOnAttributeChanged, EDS1AttributeType, float);
DECLARE_MULTICAST_DELEGATE(FOnDeath)
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DS1_API UDS1AttributeComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	//���� ���� Delegate
	FDelegateOnAttributeChanged OnAttributeChanged;

	FOnDeath OnDeath;

protected:
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float BaseStamina = 100.f;
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float MaxStamina = 100.f;
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float StaminaRegenRate = 0.2f;
	UPROPERTY(EditAnywhere, Category = "Health")
	float BaseHealth = 100.f;
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.f;
	//���¹̳� ������ Ÿ�̸� �ڵ�
	FTimerHandle StaminaRegenTimerHandle;


public:
	UDS1AttributeComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE float GetBaseStamina() const { return BaseStamina; };
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; };
	FORCEINLINE float GetStaminaRatio() const { return BaseStamina / MaxStamina; };
	FORCEINLINE float GetHealthRatio() const { return BaseHealth / MaxHealth; };
	//���¹̳ʰ� �������
	UFUNCTION()
	bool CheckHasEnoughStamina(float StaminaCost) const;
	//���¹̳� ����
	UFUNCTION()
	void DecreaseStamina(float StaminaCost);
	//���¹̳� ������ ����
	UFUNCTION()
	void ToggleStaminaRegeneration(bool bEnabled, float StartDelay = 1.f);
	UFUNCTION()
	void BroadcastAttributeChanged(EDS1AttributeType InAttributeType) const;

	void TakeDamageAmount(float DamageAmount);

	void ApplyHealthChange(float Delta);
private:
	//���¹̳� ������
	UFUNCTION()
	void RegenStaminaHandler();
};
