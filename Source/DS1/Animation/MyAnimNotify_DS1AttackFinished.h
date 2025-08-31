// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MyAnimNotify_DS1AttackFinished.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Attack Finished"))
class DS1_API UMyAnimNotify_DS1AttackFinished : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	float ComboResetDelay = 1.f;

public:
	UMyAnimNotify_DS1AttackFinished(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
