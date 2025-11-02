// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LTRotationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LT_API ULTRotationComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TargetActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldRotate = false;

public:	
	// Sets default values for this component's properties
	ULTRotationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE void SetTargetActor(AActor* InActor) { TargetActor = InActor; }
	FORCEINLINE void ToggleShouldRotate(const bool bRotate) { bShouldRotate = bRotate; }
		
};
