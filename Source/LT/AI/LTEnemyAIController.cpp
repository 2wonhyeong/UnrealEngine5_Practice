// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/LTEnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/LTCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"

ALTEnemyAIController::ALTEnemyAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");
}

void ALTEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

    ControlledEnemy = Cast<ALTEnemy>(InPawn);

	RunBehaviorTree(BehaviorTreeAsset);

    // UpdateTarget 타이머 등록
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ALTEnemyAIController::UpdateTarget, 0.1f, true);
}

void ALTEnemyAIController::OnUnPossess()
{
    ControlledEnemy = nullptr;
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	Super::OnUnPossess();
}

void ALTEnemyAIController::UpdateTarget() const
{
    TArray<AActor*> OutActors;
    AIPerceptionComponent->GetKnownPerceivedActors(nullptr, OutActors);

    ALTCharacter* PlayerCharacter = Cast<ALTCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (OutActors.Contains(PlayerCharacter))
    {
        SetTarget(PlayerCharacter);
        ControlledEnemy->ToggleHPBarVisibility(true);
    }
    else
    {
        SetTarget(nullptr);
        ControlledEnemy->ToggleHPBarVisibility(false);
    }
}
void ALTEnemyAIController::SetTarget(AActor* NewTarget) const
{
    if (IsValid(Blackboard))
    {
        Blackboard->SetValueAsObject(FName("Target"), NewTarget);
    }
}
