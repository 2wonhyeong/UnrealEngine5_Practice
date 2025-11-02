// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "LTGamePlayTags.h"
#include "LTStateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LT_API ULTStateComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	//이동키 입력 상태 관리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bMovementInputEnabled = true;
	//캐릭터 현재 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	FGameplayTag CurrentState;

public:	
	ULTStateComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//이동 입력이 활성화 상태인지
	FORCEINLINE bool MovementInputEnabled() const { return bMovementInputEnabled; };

	void ToggleMovementInput(bool bEnabled, float Duration = 0.f);

protected:
	UFUNCTION()
	void MovementInputEnableAction();

public:
	FORCEINLINE void SetState(const FGameplayTag NewState)
	{
		if (CurrentState == LTGamePlayTags::Character_State_Death)
			return;
		CurrentState = NewState;
	};
	FORCEINLINE FGameplayTag GetCurrentState() const { return CurrentState; };

	void ClearState();

	bool IsCurrentStateEqualToAny(const FGameplayTagContainer& TagsToCheck) const;
		
};
