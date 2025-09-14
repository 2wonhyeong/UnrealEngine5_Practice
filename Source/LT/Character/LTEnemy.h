// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayEffectTypes.h"
#include "Interfaces/LTTargeting.h"
#include "Interfaces/LTCombatInterface.h"
#include "Components/LTCombatComponent.h"
#include "LTEnemy.generated.h"

class UWidgetComponent;
class USphereComponent;
class ULTStateComponent;
class ULTAttributeComponent;
class ATargetPoint;

UCLASS()
class LT_API ALTEnemy : public ACharacter, public ILTTargeting, public ILTCombatInterface
{
	GENERATED_BODY()

protected:
	/** 타겟팅 충돌 체크용 */
	UPROPERTY(VisibleAnywhere)
	USphereComponent* TargetingSphereComponent;
	/** LockOn UI Widget */
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* LockOnWidgetComponent;
	UPROPERTY(VisibleAnywhere)
	ULTAttributeComponent* AttributeComponent;
	UPROPERTY(VisibleAnywhere)
	ULTStateComponent* StateComponent;
	UPROPERTY(VisibleAnywhere)
	ULTCombatComponent* CombatComponent;

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
	UPROPERTY(EditAnywhere, Category = "AI | Patrol")
	TArray<ATargetPoint*> PatrolPoints;

	UPROPERTY(VisibleAnywhere, Category = "AI | Patrol")
	int32 PatrolIndex = 0;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ALTWeapon> DefaultWeaponClass;

public:
	ALTEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual void OnDeath();

protected:
	//파티클 and 소리
	void ImpactEffect(const FVector& Location);
	//애니메이션
	void HitReaction(const AActor* Attacker);
	//애니메이션 방향
	UAnimMontage* GetHitReactAnimation(const AActor* Attacker) const;

public:
	virtual void OnTargeted(bool bTargeted) override;
	virtual bool CanBeTargeted() override;

	virtual void ActivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) override;
	virtual void DeactivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) override;
	virtual void PerformAttack(FGameplayTag& AttackTypeTag, FOnMontageEnded& MontageEndedDelegate) override;

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
