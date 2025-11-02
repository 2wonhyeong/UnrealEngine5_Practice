// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_SelectBehavior.h"
#include "Character/LTEnemy.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "LTGamePlayTags.h"
#include "Components/LTStateComponent.h"

UBTService_SelectBehavior::UBTService_SelectBehavior()
{
	INIT_SERVICE_NODE_NOTIFY_FLAGS();
}

void UBTService_SelectBehavior::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn)
		return;
	ControlledEnemy = Cast<ALTEnemy>(ControlledPawn);
}

void UBTService_SelectBehavior::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory,DeltaSeconds);

	UpdateBehavior(OwnerComp.GetBlackboardComponent());
}

void UBTService_SelectBehavior::SetBehaviorKey(UBlackboardComponent* BlackboardComp, ELTAIBehavior Behavior) const
{
	BlackboardComp->SetValueAsEnum(BehaviorKey.SelectedKeyName, static_cast<uint8>(Behavior));
}

void UBTService_SelectBehavior::UpdateBehavior(UBlackboardComponent* BlackboardComp)
{
	check(BlackboardComp);
	check(ControlledEnemy);

	const ULTStateComponent* StateComponent = ControlledEnemy->GetComponentByClass<ULTStateComponent>();
	check(StateComponent);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(LTGamePlayTags::Character_State_Parried);
	CheckTags.AddTag(LTGamePlayTags::Character_State_Stunned);

	if (StateComponent->IsCurrentStateEqualToAny(CheckTags))
	{
		SetBehaviorKey(BlackboardComp, ELTAIBehavior::Stunned);
	}
	else
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
		if (IsValid(TargetActor)) {
			const float Distance = TargetActor->GetDistanceTo(ControlledEnemy);
			//공격범위 안쪽이면
			if (Distance <= AttackRangeDistance)
			{
				SetBehaviorKey(BlackboardComp, ELTAIBehavior::MeleeAttack);
			}
			else
			{
				SetBehaviorKey(BlackboardComp, ELTAIBehavior::Approach);
			}
		}
		else
		{
			//patrol point 있으면
			if (ControlledEnemy->GetPatrolPoint() != nullptr)
			{
				SetBehaviorKey(BlackboardComp, ELTAIBehavior::Patrol);
			}
			else
			{
				SetBehaviorKey(BlackboardComp, ELTAIBehavior::Idle);
			}
		}
	}
}
