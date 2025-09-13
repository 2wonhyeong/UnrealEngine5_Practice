// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolPoint.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/LTEnemy.h"
#include "Engine/TargetPoint.h"

EBTNodeResult::Type UBTTask_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (!ControlledPawn)
    {
        return EBTNodeResult::Failed;
    }

    if (ALTEnemy* Character = Cast<ALTEnemy>(ControlledPawn))
    {
        // TargetPoint의 Location을 Blackboard에 저장.
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardLocation.SelectedKeyName, Character->GetPatrolPoint()->GetActorLocation());
        Character->IncrementPatrolIndex();
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}