// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_PerformAttack.h"
#include "Components/LTStateComponent.h"
#include "Interfaces/LTCombatInterface.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_PerformAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn)
		return EBTNodeResult::Failed;

	if (ILTCombatInterface* CombatInterface = Cast<ILTCombatInterface>(ControlledPawn))
	{
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindLambda([this, &OwnerComp, ControlledPawn](UAnimMontage* Montage, bool bInterrupted)
			{
				UE_LOG(LogTemp, Log, TEXT("Execute MontageEndedDelegate"));
				if (IsValid(&OwnerComp) == false) { return; }
				if (ULTStateComponent* StateComponent = ControlledPawn->GetComponentByClass<ULTStateComponent>())
				{
					StateComponent->ClearState();
				}
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			});
		CombatInterface->PerformAttack(AttackTypeTag, MontageEndedDelegate);
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}
