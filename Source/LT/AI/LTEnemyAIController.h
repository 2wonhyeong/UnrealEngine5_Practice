// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LTEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class LT_API ALTEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviorTreeAsset;

	/** AI�� �ֺ�ȯ���� �ν��� �� �ְ� ���ִ� ������Ʈ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* AIPerceptionComponent;

	FTimerHandle TimerHandle;

public:
	ALTEnemyAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

protected:
	void UpdateTarget() const;
	void SetTarget(AActor* NewTarget) const;
};
