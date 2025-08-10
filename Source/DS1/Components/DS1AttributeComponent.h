// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DS1Define.h"
#include "DS1AttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FDelegateOnAttrivuteChanged, EDS1AttributeType, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DS1_API UDS1AttributeComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	//���� ���� Delegate
	FDelegateOnAttrivuteChanged OnAttributeChanged;

protected:
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float BaseStamina = 100.f;
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float MaxStamina = 100.f;
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float StaminaRegenRate = 0.2f;
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
	//���¹̳ʰ� �������
	UFUNCTION()
	bool CheckHasEnoughStamina(float StaminaCost) const;
	//���¹̳� ����
	UFUNCTION()
	void DecreaseStamina(float StaminaCost);
	//���¹̳� ������ ����
	UFUNCTION()
	void ToggleStaminaRegeneration(bool bEnabled, float StartDelay = 2.f);
	UFUNCTION()
	void BroadcastAttributeChanged(EDS1AttributeType InAttributeType) const;
private:
	//���¹̳� ������
	UFUNCTION()
	void RegenStaminaHandler();
};
