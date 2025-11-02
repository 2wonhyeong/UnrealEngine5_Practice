// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_LTRotate.h"
#include "Components/LTRotationComponent.h"
#include "AnimNotifyState_LTRotate.h"

void UAnimNotifyState_LTRotate::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ULTRotationComponent* RotationComponent = MeshComp->GetOwner()->GetComponentByClass<ULTRotationComponent>())
		RotationComponent->ToggleShouldRotate(true);
}

void UAnimNotifyState_LTRotate::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ULTRotationComponent* RotationComponent = MeshComp->GetOwner()->GetComponentByClass<ULTRotationComponent>())
		RotationComponent->ToggleShouldRotate(false);
}
