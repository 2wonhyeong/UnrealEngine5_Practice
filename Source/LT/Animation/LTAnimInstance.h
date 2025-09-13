// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LTDefine.h"
#include "Animation/AnimInstance.h"
#include "LTAnimInstance.generated.h"

class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class LT_API ULTAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	ACharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	UCharacterMovementComponent* MovementComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	bool bShouldMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	bool bIsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Data")
	bool bCombatEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Data")
	ECombatType CombatType = ECombatType::None;

public:
	ULTAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	UFUNCTION()
	void AnimNotify_ResetMovementInput();
	UFUNCTION()
	void AnimNotify_ResetState();

public:
	void UpdateCombatMode(const ECombatType InCombatType);

protected:
	void OnChangedCombat(const bool bInCombatEnabled);
};
