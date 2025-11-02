// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/LT_PotionDrink.h"
#include "Components/LTPotionInventoryComponent.h"

void ULT_PotionDrink::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (ULTPotionInventoryComponent* PotionInventory = OwnerActor->GetComponentByClass<ULTPotionInventoryComponent>())
		{
			PotionInventory->DrinkPotion();
		}
	}
}
