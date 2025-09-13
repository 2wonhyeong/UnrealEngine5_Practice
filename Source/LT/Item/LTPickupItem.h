// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/LTInteract.h"
#include "LTPickupItem.generated.h"
class ALTEquipment;
UCLASS()
class LT_API ALTPickupItem : public AActor, public ILTInteract
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALTPickupItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Interact(AActor* InteractionActor) override;

public:
	FORCEINLINE void SetEquipmentClass(const TSubclassOf<ALTEquipment>& NewEquipmentClass) { EquipmentClass = NewEquipmentClass; };

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Item")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TSubclassOf<ALTEquipment> EquipmentClass;
};