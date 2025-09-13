// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MyAnimNotify_LTAttackFinished.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Attack Finished"))
class LT_API UMyAnimNotify_LTAttackFinished : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	float ComboResetDelay = 1.f;

public:
	UMyAnimNotify_LTAttackFinished(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
