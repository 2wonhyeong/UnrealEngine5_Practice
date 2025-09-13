// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LTDefine.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LTTargetingComponent.generated.h"


class UCameraComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LT_API ULTTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	/** LockOn ��ų �ĺ����� ������ ������ ũ��(������) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetingRadius = 500.f;

	/** LockOn�� ī�޶� ȸ�� �ӵ� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FaceLockOnRotationSpeed = 20.f;

	/** ����� ��ο� Ÿ�� */
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;

protected:
	UPROPERTY()
	ACharacter* Character;

	UPROPERTY()
	UCameraComponent* Camera;

	/** ���� LockOn�� ��� */
	UPROPERTY()
	AActor* LockedTargetActor;

	/** LockOn ���� ���� */
	bool bIsLockOn = false;

public:
	ULTTargetingComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/** LockedOn On/Off */
	void ToggleLockOn();

	/** LockOn ��� ����Ī */
	void SwitchingLockedOnActor(ESwitchingDirection InDirection);

	/** LockOn ���� */
	FORCEINLINE bool IsLockOn() const { return bIsLockOn; }

protected:
	/** LockOn ��ų �ĺ����� ã���ϴ�. */
	void FindTargets(OUT TArray<AActor*>& OutTargetingActors) const;

	/** ���� LockOn ��ų ����� ã�� �ݴϴ�. */
	AActor* FindClosestTarget(TArray<AActor*>& InTargets, ESwitchingDirection InDirection = ESwitchingDirection::None) const;

	/** ĳ���Ͱ� ī�޶�(ControlRotation)�� ȸ���� ����ȭ �ǵ��� ���� */
	void OrientCamera() const;

	/** ĳ���Ͱ� �̵� �������� ȸ���ϵ��� ���� */
	void OrientMovement() const;

	/** LockedOn ���¿��� ī�޶� ȸ�� ���� */
	void FaceLockOnActor() const;

	/** LockOn */
	void LockOnTarget();

	/** LockOn ���� */
	void StopLockOn();
};
