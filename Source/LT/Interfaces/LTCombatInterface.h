// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LTDefine.h"
#include "UObject/Interface.h"
#include "LTCombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULTCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LT_API ILTCombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ActivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) = 0;
	virtual void DeactivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) = 0;
};
