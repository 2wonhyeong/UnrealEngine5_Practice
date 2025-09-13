// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "LTEquipWeapon.generated.h"

/**
 *
 */
UCLASS(meta = (DisplayName = "Equip Weapon"))
class LT_API ULTEquipWeapon : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag MontageActionTag;

public:
	ULTEquipWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
