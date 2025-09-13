// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/LTEquipment.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ALTEquipment::ALTEquipment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquipmentMesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

} 

// Called when the game starts or when spawned
void ALTEquipment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALTEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALTEquipment::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (MeshAsset)
	{
		Mesh->SetStaticMesh(MeshAsset);
	}
}

void ALTEquipment::EquipItem()
{
}

void ALTEquipment::UnequipItem()
{
}

void ALTEquipment::AttachToOwner(FName SocketName)
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh())
		{
			AttachToComponent(CharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), SocketName);
		}
	}
}

