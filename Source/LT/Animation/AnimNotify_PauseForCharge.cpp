// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_PauseForCharge.h"
#include "Character/LTEnemy.h"

void UAnimNotify_PauseForCharge::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 이 몽타주를 재생하는 오너(ALTEnemy)를 찾습니다.
	if (ALTEnemy* Enemy = Cast<ALTEnemy>(MeshComp->GetOwner()))
	{
		// 적에게 "지금 일시정지하고 선딜 타이머 시작해!"라고 명령합니다.
		Enemy->PauseForCharge();
	}
}

