// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayEffectTypes.h"
#include "Interfaces/LTTargeting.h"
#include "Interfaces/LTCombatInterface.h"
#include "Components/LTCombatComponent.h"
#include "Components/LTRotationComponent.h"
#include "LTEnemy.generated.h"

class UWidgetComponent;
class USphereComponent;
class ULTStateComponent;
class ULTAttributeComponent;
class ATargetPoint;
class ULTRotationComponent;

UCLASS()
class LT_API ALTEnemy : public ACharacter, public ILTTargeting, public ILTCombatInterface
{
	GENERATED_BODY()

protected:
	/** 타겟팅 충돌 체크용 */
	UPROPERTY(VisibleAnywhere)
	USphereComponent* TargetingSphereComponent;
	UPROPERTY(VisibleAnywhere)
	ULTAttributeComponent* AttributeComponent;
	UPROPERTY(VisibleAnywhere)
	ULTStateComponent* StateComponent;
	UPROPERTY(VisibleAnywhere)
	ULTCombatComponent* CombatComponent;
	/** LockOn UI Widget */
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* LockOnWidgetComponent;
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* HPBarWidgetComponent;
	UPROPERTY(VisibleAnywhere)
	ULTRotationComponent* RotationComponent;

	// Effect Section
protected:
	UPROPERTY(EditAnywhere, Category = "Effect")
	USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* ImpactParticle;

	// Montage Section
/*protected:
	UPROPERTY(EditAnywhere, Category = "Montage | HitReact")
	UAnimMontage* HitReactAnimFront;

	UPROPERTY(EditAnywhere, Category = "Montage | HitReact")
	UAnimMontage* HitReactAnimBack;

	UPROPERTY(EditAnywhere, Category = "Montage | HitReact")
	UAnimMontage* HitReactAnimLeft;

	UPROPERTY(EditAnywhere, Category = "Montage | HitReact")
	UAnimMontage* HitReactAnimRight;*/

protected:
	UPROPERTY(EditAnywhere, Category = "AI | Patrol")
	TArray<ATargetPoint*> PatrolPoints;

	UPROPERTY(VisibleAnywhere, Category = "AI | Patrol")
	int32 PatrolIndex = 0;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stun Duration")
	float StunnedDelay = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "sundeal")
	float HoldDuration = 1.5f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ALTWeapon> DefaultWeaponClass;

protected:
	FTimerHandle ParriedDelayTimerHandle;
	FTimerHandle StunnedDelayTimerHandle;
	FTimerHandle HoldAttackTimerHandle;
	FTimerHandle SpecialAttackCooldownTimerHandle;
	UPROPERTY()
	UAnimMontage* CurrentAttackMontage;

public:
	ALTEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	virtual void OnDeath();
	void SetDeathState();
	void OnAttributeChanged(ELTAttributeType AttributeType, float InValue);
	void SetupHpBar();

protected:
	//파티클 and 소리
	void ImpactEffect(const FVector& Location);
	//애니메이션
	void HitReaction(const AActor* Attacker);
	//애니메이션 방향
	//UAnimMontage* GetHitReactAnimation(const AActor* Attacker) const;

public:
	virtual void OnTargeted(bool bTargeted) override;
	virtual bool CanBeTargeted() override;

	virtual void ActivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) override;
	virtual void DeactivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) override;
	virtual void PerformAttack(FGameplayTag& AttackTypeTag, FOnMontageEnded& MontageEndedDelegate) override;
	virtual void Parried() override;
	void PauseForCharge();

	void ToggleHPBarVisibility(bool bVisibility);

protected:
	//선딜 대기 시간이 끝났을 때 실행
	void ReleasedAttack();
	UFUNCTION()
	void OnSpecialAttackCooldownFinished();

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
