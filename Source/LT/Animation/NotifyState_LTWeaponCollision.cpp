// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState_LTWeaponCollision.h"

#include "Interfaces/LTCombatInterface.h"

UNotifyState_LTWeaponCollision::UNotifyState_LTWeaponCollision(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UNotifyState_LTWeaponCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (ILTCombatInterface* CombatInterface = Cast<ILTCombatInterface>(OwnerActor))
		{
			CombatInterface->ActivateWeaponCollision(CollisionType);
		}
	}
}

void UNotifyState_LTWeaponCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (ILTCombatInterface* CombatInterface = Cast<ILTCombatInterface>(OwnerActor))
		{
			CombatInterface->DeactivateWeaponCollision(CollisionType);
		}
	}
}
