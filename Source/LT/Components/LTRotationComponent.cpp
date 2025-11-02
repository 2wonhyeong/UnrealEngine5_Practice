// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LTRotationComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
ULTRotationComponent::ULTRotationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULTRotationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULTRotationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!TargetActor)
		return;
	if (!bShouldRotate)
		return;
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), TargetActor->GetActorLocation());
	GetOwner()->SetActorRotation(FRotator(0.f, LookAtRotation.Yaw, 0.f));
}

