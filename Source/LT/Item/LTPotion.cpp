// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/LTPotion.h"

// Sets default values
ALTPotion::ALTPotion()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PotionMesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

