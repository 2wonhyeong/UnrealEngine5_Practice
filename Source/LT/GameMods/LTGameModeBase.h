// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LTGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class LT_API ALTGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    UFUNCTION(Exec)
    void PlayerDied(APlayerController* PlayerController);

protected:
    UFUNCTION()
    void RespawnPlayer(AController* Controller);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
    float RespawnDelay = 3.0f;
};
