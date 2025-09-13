// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_LTComboWindow.h"
#include "Character/LTCharacter.h"

UAnimNotifyState_LTComboWindow::UAnimNotifyState_LTComboWindow(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UAnimNotifyState_LTComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ALTCharacter* Character = Cast<ALTCharacter>(MeshComp->GetOwner()))
	{
		Character->EnableComboWindow();
	}
}

void UAnimNotifyState_LTComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ALTCharacter* Character = Cast<ALTCharacter>(MeshComp->GetOwner()))
	{
		Character->DisableComboWindow();
	}
}
