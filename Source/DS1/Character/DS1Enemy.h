// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayEffectTypes.h"
#include "Interfaces/DS1Targeting.h"
#include "Engine/TargetPoint.h"
#include "DS1Enemy.generated.h"

class UWidgetComponent;
class USphereComponent;
class UDS1StateComponent;
class UDS1AttributeComponent;

UCLASS()
class DS1_API ADS1Enemy : public ACharacter, public IDS1Targeting
{
	GENERATED_BODY()

protected:
	/** Ÿ���� �浹 üũ�� */
	UPROPERTY(VisibleAnywhere)
	USphereComponent* TargetingSphereComponent;
	/** LockOn UI Widget */
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* LockOnWidgetComponent;
	UPROPERTY(VisibleAnywhere)
	UDS1AttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	UDS1StateComponent* StateComponent;

	// Effect Section
protected:
	UPROPERTY(EditAnywhere, Category = "Effect")
	USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* ImpactParticle;

	// Montage Section
protected:
	UPROPERTY(EditAnywhere, Category = "Montage | HitReact")
	UAnimMontage* HitReactAnimFront;

	UPROPERTY(EditAnywhere, Category = "Montage | HitReact")
	UAnimMontage* HitReactAnimBack;

	UPROPERTY(EditAnywhere, Category = "Montage | HitReact")
	UAnimMontage* HitReactAnimLeft;

	UPROPERTY(EditAnywhere, Category = "Montage | HitReact")
	UAnimMontage* HitReactAnimRight;

protected:
	//����������
	UPROPERTY(EditAnywhere, Category = "AI | Patrol")
	TArray<ATargetPoint*> PatrolPoints;
	//���� ��Ʈ�� ����Ʈ
	UPROPERTY(VisibleAnywhere, Category = "AI | Patrol")
	int32 PatrolIndex = 0;

public:
	ADS1Enemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual void OnDeath();

protected:
	//��ƼŬ and �Ҹ�
	void ImpactEffect(const FVector& Location);
	//�ִϸ��̼�
	void HitReaction(const AActor* Attacker);
	//�ִϸ��̼� ����
	UAnimMontage* GetHitReactAnimation(const AActor* Attacker) const;

public:
	virtual void OnTargeted(bool bTargeted) override;
	virtual bool CanBeTargeted() override;

public:
	FORCEINLINE ATargetPoint* GetPatrolPoint()
	{
		return PatrolPoints.Num() >= (PatrolIndex + 1) ? PatrolPoints[PatrolIndex] : nullptr;
	}
	FORCEINLINE void IncrementPatrolIndex()
	{
		PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();
	}
};
