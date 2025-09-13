// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MyAnimNotify_LTAttackFinished.h"

#include "Character/LTCharacter.h"
#include "Components/LTStateComponent.h"

UMyAnimNotify_LTAttackFinished::UMyAnimNotify_LTAttackFinished(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UMyAnimNotify_LTAttackFinished::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ALTCharacter* Character = Cast<ALTCharacter>(MeshComp->GetOwner()))
	{
		Character->AttackFinished(ComboResetDelay);
	}
}
