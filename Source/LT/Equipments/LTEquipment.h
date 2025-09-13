// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LTEquipment.generated.h"

UCLASS()
class LT_API ALTEquipment : public AActor
{
	GENERATED_BODY()
public:
	//장비아이템의 외형 표시
	UPROPERTY(EditAnywhere, Category="Equipment | Mesh")
	UStaticMesh* MeshAsset;
	//실제로 렌더링
	UPROPERTY(EditAnywhere, Category = "Equipment | Mesh")
	UStaticMeshComponent* Mesh;
	//해당 2개를 연결해줘야됨
public:	
	// Sets default values for this actor's properties
	ALTEquipment();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual void EquipItem();

	virtual void UnequipItem();

	virtual void AttachToOwner(FName SocketName);

};
