// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipments/LTWeapon.h"
#include "LTFistWeapon.generated.h"

/**
 * 
 */
UCLASS()
class LT_API ALTFistWeapon : public ALTWeapon
{
	GENERATED_BODY()

public:
	ALTFistWeapon();

public:
	virtual void EquipItem() override;
	
};
