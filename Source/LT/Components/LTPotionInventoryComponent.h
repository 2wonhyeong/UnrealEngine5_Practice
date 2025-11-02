// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LTPotionInventoryComponent.generated.h"

class ALTPotion;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LT_API ULTPotionInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PotionQuantity = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PotionHealAmount = 40.f;
	UPROPERTY(EditAnywhere)
	FName PotionSocketName;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ALTPotion> PotionClass;
	UPROPERTY(EditAnywhere)
	ALTPotion* PotionActor;

public:	
	// Sets default values for this component's properties
	ULTPotionInventoryComponent();

	void DrinkPotion();
	void SpawnPotion();
	void DespawnPotion();
	void SetPotionQuantity(int32 InQuantity);
	FORCEINLINE int GetPotionQuantity() const { return PotionQuantity; }
		
};
