// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_LTPotionSpawn.h"
#include "Components/LTPotionInventoryComponent.h"

void UAnimNotifyState_LTPotionSpawn::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (ULTPotionInventoryComponent* PotionInventory = OwnerActor->GetComponentByClass<ULTPotionInventoryComponent>())
		{
			PotionInventory->SpawnPotion();
		}
	}
}

void UAnimNotifyState_LTPotionSpawn::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (ULTPotionInventoryComponent* PotionInventory = OwnerActor->GetComponentByClass<ULTPotionInventoryComponent>())
		{
			PotionInventory->DespawnPotion();
		}
	}
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
