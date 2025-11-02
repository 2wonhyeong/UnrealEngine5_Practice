// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LTPotion.generated.h"

UCLASS()
class LT_API ALTPotion : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	
public:	
	// Sets default values for this actor's properties
	ALTPotion();
};
