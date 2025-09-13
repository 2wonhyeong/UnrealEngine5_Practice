// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LTDefine.h"
#include "Components/LTStateComponent.h"
#include "LTGamePlayTags.h"
#include "LTAttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FDelegateOnAttributeChanged, ELTAttributeType, float);
DECLARE_MULTICAST_DELEGATE(FOnDeath)
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LT_API ULTAttributeComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	//스탯 변경 Delegate
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
	//스태미나 재충전 타이머 핸들
	FTimerHandle StaminaRegenTimerHandle;


public:
	ULTAttributeComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE float GetBaseStamina() const { return BaseStamina; };
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; };
	FORCEINLINE float GetStaminaRatio() const { return BaseStamina / MaxStamina; };
	FORCEINLINE float GetHealthRatio() const { return BaseHealth / MaxHealth; };
	//스태미너가 충분한지
	UFUNCTION()
	bool CheckHasEnoughStamina(float StaminaCost) const;
	//스태미너 차감
	UFUNCTION()
	void DecreaseStamina(float StaminaCost);
	//스태미너 재충전 금지
	UFUNCTION()
	void ToggleStaminaRegeneration(bool bEnabled, float StartDelay = 1.f);
	UFUNCTION()
	void BroadcastAttributeChanged(ELTAttributeType InAttributeType) const;

	void TakeDamageAmount(float DamageAmount);

	void ApplyHealthChange(float Delta);
private:
	//스태미너 재충전
	UFUNCTION()
	void RegenStaminaHandler();
};
