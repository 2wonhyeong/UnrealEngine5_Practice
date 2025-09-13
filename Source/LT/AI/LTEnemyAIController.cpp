// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/LTEnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/LTCharacter.h"
#include "Character/LTEnemy.h"
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

	RunBehaviorTree(BehaviorTreeAsset);

    // UpdateTarget 타이머 등록
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::UpdateTarget, 0.1f, true);
}

void ALTEnemyAIController::OnUnPossess()
{
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
    }
    else
    {
        SetTarget(nullptr);
    }
}
void ALTEnemyAIController::SetTarget(AActor* NewTarget) const
{
    if (IsValid(Blackboard))
    {
        Blackboard->SetValueAsObject(FName("Target"), NewTarget);
    }
}
