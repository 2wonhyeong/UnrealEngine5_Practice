// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "UI/LTPlayerHUDWidget.h"
#include "Interfaces/LTCombatInterface.h"
#include "LTCharacter.generated.h"

class ALTFistWeapon;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class ULTAttributeComponent;
class ULTStateComponent;
class ULTCombatComponent;
class ULTTargetingComponent;

UCLASS()
class LT_API ALTCharacter : public ACharacter, public ILTCombatInterface
{
	GENERATED_BODY()

private:
	//캐릭터를 3인칭으로 바라볼 수 있게 해주는 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	//캐릭터와 카메라의 거리를 조절해주는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	//캐릭터의 움직임을 종합적으로 관리해주는 입력 매핑 컨텍스트
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;
	//움직임을 관리하는 입력액션
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;
	//카메라 시점 조절하는 입력액션
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;
	//달리기
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SprintAction;
	//구르기
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RollingAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;
	//공격 3 종류 처리
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackAction;
	//공격 나머지 1종류 처리
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* HeavyAttackAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "True"))
	UInputAction* TestDamageAction;
	//전투 활성화 비활성화
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "True"))
	UInputAction* ToggleCombatAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "True"))
	UInputAction* LockOnTargetAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "True"))
	UInputAction* LeftTargetActiron;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "True"))
	UInputAction* RightTargetAction;
private:
	//캐릭터 스탯 관리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	ULTAttributeComponent* AttributeComponent;
	//캐릭터 상태 관리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	ULTStateComponent* StateComponent;
	//무기, 전투 관리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	ULTCombatComponent* CombatComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	ULTTargetingComponent* TargetingComponent;
	
protected:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;
	UPROPERTY()
	ULTPlayerHUDWidget* PlayerHUDWidget;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> DeathWidgetClass;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ALTFistWeapon> FistWeaponClass;

protected:
	//질주속도
	UPROPERTY(EditAnywhere, Category = "Sprinting")
	float SprintingSpeed = 1000.f;
	//기본속도
	UPROPERTY(EditAnywhere, Category = "Sprinting")
	float NormalSpeed = 500.f;
	UPROPERTY(EditAnywhere, Category = "Sprinting")
	bool bSprinting = false;

protected:
	UPROPERTY(EditAnywhere, Category = "Effect")
	USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* ImpactParticle;

protected:
	//콤보가 진행중인지
	bool bComboSequenceRunning = false;
	//콤보가 입력 가능한지
	bool bCanComboInput = false;
	//몇번째 콤보를 실행해야하는지
	int32 ComboCounter = 0;
	//다음 콤보를 이어갈 수 있는지
	bool bSavedComboInput = false;
	//콤보 리셋
	FTimerHandle ComboResetTimerHandle;
protected:
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* RollingMontage;

public:
	// Sets default values for this character's properties
	ALTCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;

	virtual void Landed(const FHitResult& Hit) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	//파티클 and 소리
	void ImpactEffect(const FVector& Location);
	//애니메이션
	void HitReaction(const AActor* Attacker);
	virtual void OnDeath();

public:
	FORCEINLINE ULTStateComponent* GetStateComponent()const { return StateComponent; };

protected:
	FORCEINLINE bool IsSprinting() const { return bSprinting; };
	UFUNCTION()
	void Move(const FInputActionValue& Values);
	UFUNCTION()
	void Look(const FInputActionValue& Values);
	UFUNCTION()
	void Sprinting();
	UFUNCTION()
	void StopSprint();
	UFUNCTION()
	bool IsMoving() const;
	UFUNCTION()
	void Rolling();
	void Interact();
	bool CanToggleCombat() const;
	void ToggleCombat();
	void ApplyDamage();
	//무기를 등에 차고 있는 상태에서 바로 공격할 수 있게
	void AutoToggleCombat();
	void Attack();
	void SpecialAttack();
	void HeavyAttack();
	void Jumping();
	bool CanPerformJump() const;
	bool CanPerformRoll() const;
	void LockOnTarget();
	void LeftTarget();
	void RightTarget();
protected:
	//현재 상태에서 가능한 일반공격
	FGameplayTag GetAttackPerform() const;

	//공격 가능 조건 체크
	bool CanPerformAttack(const FGameplayTag& AttackTypeTag) const;
	//공격 실행
	void DoAttack(const FGameplayTag& AttackTypeTag);
	//콤보 실행
	void ExecuteComboAttack(const FGameplayTag& AttackTypeTag);
	//콤보 초기화
	void ResetCombo();

public:
	void EnableComboWindow();
	void DisableComboWindow();
	void AttackFinished(const float ComboResetDelay);

public:
	virtual void ActivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) override;
	virtual void DeactivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) override;
};