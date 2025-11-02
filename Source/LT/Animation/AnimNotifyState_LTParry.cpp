// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_LTParry.h"

#include "Components/LTStateComponent.h"

void UAnimNotifyState_LTParry::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (ULTStateComponent* StateComponent = OwnerActor->GetComponentByClass<ULTStateComponent>())
		{
			StateComponent->SetState(LTGamePlayTags::Character_State_Parrying);
		}
	}
}

void UAnimNotifyState_LTParry::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (ULTStateComponent* StateComponent = OwnerActor->GetComponentByClass<ULTStateComponent>())
		{
			StateComponent->ClearState();
		}
	}
}
