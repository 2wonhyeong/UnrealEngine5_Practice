// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/LTPickupItem.h"
#include "Equipments/LTEquipment.h"
#include "LTDefine.h"

// Sets default values
ALTPickupItem::ALTPickupItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupItemMesh"));
	SetRootComponent(Mesh);

	Mesh->SetCollisionObjectType(COLLISION_OBJECT_INTERACTION);
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

// Called when the game starts or when spawned
void ALTPickupItem::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ALTPickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALTPickupItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (EquipmentClass)
	{
		if (ALTEquipment* CDO = EquipmentClass->GetDefaultObject<ALTEquipment>())
		{
			Mesh->SetStaticMesh(CDO->MeshAsset);
			Mesh->SetSimulatePhysics(true);
		}
	}
}

void ALTPickupItem::Interact(AActor* InteractionActor)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = InteractionActor;

	ALTEquipment* SpawnItem = GetWorld()->SpawnActor<ALTEquipment>(EquipmentClass, GetActorTransform(), SpawnParams);
	if (SpawnItem)
	{
		SpawnItem->EquipItem();
		Destroy();
	}
}