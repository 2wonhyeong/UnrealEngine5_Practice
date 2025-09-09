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
	//비해이비어 트리 연결
	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviorTreeAsset;
	//AI가 주변환경을 인식할수있게해줌
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
