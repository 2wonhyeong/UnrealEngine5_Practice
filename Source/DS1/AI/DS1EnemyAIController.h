// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DS1EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class DS1_API ADS1EnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	//�����̺�� Ʈ�� ����
	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviorTreeAsset;
	//AI�� �ֺ�ȯ���� �ν��Ҽ��ְ�����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* AIPerceptionComponent;

	FTimerHandle TimerHandle;

public:
	ADS1EnemyAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

protected:
	void UpdateTarget() const;
	void SetTarget(AActor* NewTarget) const;
	
};
