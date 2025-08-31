// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DS1TargetingComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/DS1Targeting.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UDS1TargetingComponent::UDS1TargetingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDS1TargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		Camera = Character->GetComponentByClass<UCameraComponent>();
	}

}

void UDS1TargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsLockOn == false)
	{
		return;
	}

	if (IsValid(Character) == false)
	{
		return;
	}

	if (IsValid(LockedTargetActor) == false)
	{
		return;
	}

	const float Distance = FVector::Distance(Character->GetActorLocation(), LockedTargetActor->GetActorLocation());

	if (IDS1Targeting* Targeting = Cast<IDS1Targeting>(LockedTargetActor))
	{
		// TargetingRadius ���� �־����� LockedOn�� �ߴ��Ѵ�.
		if (Targeting->CanBeTargeted() == false || Distance > TargetingRadius)
		{
			StopLockOn();
		}
		else
		{
			FaceLockOnActor();
		}
	}

}

void UDS1TargetingComponent::ToggleLockOn()
{
	if (bIsLockOn)
	{
		StopLockOn();
	}
	else
	{
		LockOnTarget();
	}
}

void UDS1TargetingComponent::SwitchingLockedOnActor(ESwitchingDirection InDirection)
{
	if (::IsValid(LockedTargetActor))
	{
		if (IDS1Targeting* Targeting = Cast<IDS1Targeting>(LockedTargetActor))
		{
			// ���� Ÿ�� ����.
			Targeting->OnTargeted(false);

			// �ű� Ÿ�� ����.
			TArray<AActor*> OutTargets;
			FindTargets(OutTargets);
			AActor* TargetActor = FindClosestTarget(OutTargets, InDirection);

			if (::IsValid(TargetActor))
			{
				if (IDS1Targeting* NewTargeting = Cast<IDS1Targeting>(TargetActor))
				{
					LockedTargetActor = TargetActor;
					NewTargeting->OnTargeted(true);
				}
			}
			else
			{
				StopLockOn();
			}
		}
	}
}

void UDS1TargetingComponent::FindTargets(OUT TArray<AActor*>& OutTargetingActors) const
{
	TArray<FHitResult> OutHits;
	const FVector Start = Character->GetActorLocation();
	const FVector End = Start;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_OBJECT_TARGETING));

	TArray<AActor*> ActorsToIgnore;

	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetOwner(),
		Start,
		End,
		TargetingRadius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		DrawDebugType,
		OutHits,
		true);

	if (bHit)
	{
		for (const FHitResult& HitResult : OutHits)
		{
			AActor* HitActor = HitResult.GetActor();
			if (IDS1Targeting* Targeting = Cast<IDS1Targeting>(HitActor))
			{
				if (Targeting->CanBeTargeted())
				{
					OutTargetingActors.Add(HitActor);
				}
			}
		}
	}
}

AActor* UDS1TargetingComponent::FindClosestTarget(TArray<AActor*>& InTargets, ESwitchingDirection InDirection) const
{
	float TargetCompareValue = 0.f;
	AActor* ClosestTarget = nullptr;

	for (const AActor* TargetActor : InTargets)
	{
		if (InDirection != ESwitchingDirection::None && LockedTargetActor == TargetActor)
		{
			continue;
		}

		FHitResult OutHit;
		const FVector Start = Camera->GetComponentLocation();
		const FVector End = TargetActor->GetActorLocation();

		TArray<AActor*> ActorsToIgnore;


		// ����� ECC_Visibility ä�ο� ���ؼ� Response�� Block���� �����Ǿ� �ִ��� Ȯ�� �ʿ�.
		const bool bHit = UKismetSystemLibrary::LineTraceSingle(
			GetOwner(),
			Start,
			End,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			ActorsToIgnore,
			DrawDebugType,
			OutHit,
			true);

		if (bHit)
		{
			// ī�޶��� �þ߸� �������� �ϱ� ������ ���� ����� ī�޶� �������� ó���մϴ�.
			// Left ���⿡ ���� ���� ���.
			if (InDirection == ESwitchingDirection::Left)
			{
				// �񱳴�� Vector�� Normal �����̱� ������ �ݴ�� ���˴ϴ�.
				if (FVector::DotProduct(Camera->GetRightVector(), OutHit.Normal) > 0.f == false)
				{
					continue;
				}
			}

			// Right ���⿡ ���� ���� ���.
			if (InDirection == ESwitchingDirection::Right)
			{
				if (FVector::DotProduct(Camera->GetRightVector(), OutHit.Normal) < 0.f == false)
				{
					continue;
				}
			}

			AActor* HitActor = OutHit.GetActor();

			// �÷��̾�� ���� ����� Ÿ���� ã�´�.
			// ���濡 �ִ� ���� ����� Ÿ���� ã�� ���ؼ� ī�޶��� ForwardVector�� LookAt Vector�� �����մϴ�.
			const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), HitActor->GetActorLocation());
			float CheckValue = FVector::DotProduct(Camera->GetForwardVector(), LookAtRotation.Vector());

			if (CheckValue > TargetCompareValue)
			{
				TargetCompareValue = CheckValue;
				ClosestTarget = HitActor;
			}
		}
	}

	return ClosestTarget;
}

void UDS1TargetingComponent::OrientCamera() const
{
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void UDS1TargetingComponent::OrientMovement() const
{
	Character->GetCharacterMovement()->bOrientRotationToMovement = true;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void UDS1TargetingComponent::FaceLockOnActor() const
{
	const FRotator CurrentControlRotation = Character->GetControlRotation();

	const FVector TargetLocation = LockedTargetActor->GetActorLocation() - FVector(0.f, 0.f, 150.f);
	const FRotator TargetLookAtRotation = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), TargetLocation);

	FRotator InterpRotation = FMath::RInterpTo(CurrentControlRotation, TargetLookAtRotation, GetWorld()->GetDeltaSeconds(), FaceLockOnRotationSpeed);

	// Roll�� ���� ControlRotation.Roll������ �����ϰ� Pitch/Yaw ���� InterpRotation�� ������ ����.
	Character->GetController()->SetControlRotation(FRotator(InterpRotation.Pitch, InterpRotation.Yaw, CurrentControlRotation.Roll));
}

void UDS1TargetingComponent::LockOnTarget()
{
	TArray<AActor*> OutTargets;
	FindTargets(OutTargets);
	AActor* TargetActor = FindClosestTarget(OutTargets);

	if (::IsValid(TargetActor))
	{
		LockedTargetActor = TargetActor;
		bIsLockOn = true;
		if (IDS1Targeting* Targeting = Cast<IDS1Targeting>(LockedTargetActor))
		{
			Targeting->OnTargeted(bIsLockOn);
		}

		OrientCamera();
	}
}

void UDS1TargetingComponent::StopLockOn()
{
	bIsLockOn = false;
	if (IDS1Targeting* Targeting = Cast<IDS1Targeting>(LockedTargetActor))
	{
		Targeting->OnTargeted(bIsLockOn);
	}
	LockedTargetActor = nullptr;
	OrientMovement();
}

