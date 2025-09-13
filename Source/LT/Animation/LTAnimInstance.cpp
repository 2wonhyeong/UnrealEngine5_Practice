// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/LTAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/LTCombatComponent.h"
#include "Character/LTCharacter.h"
#include "KismetAnimationLibrary.h"
#include "Components/LTStateComponent.h"

ULTAnimInstance::ULTAnimInstance()
{
}

void ULTAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    Character = Cast<ACharacter>(GetOwningActor());

    if (Character)
    {
        MovementComponent = Character->GetCharacterMovement();
        if (ULTCombatComponent* CombatComponent = Character->GetComponentByClass<ULTCombatComponent>())
        {
            CombatComponent->OnChangedCombat.AddUObject(this, &ULTAnimInstance::OnChangedCombat);
        }
    }
}

void ULTAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (Character == nullptr)
    {
        return;
    }

    if (MovementComponent == nullptr)
    {
        return;
    }

    Velocity = MovementComponent->Velocity;
    GroundSpeed = Velocity.Size2D();

    bShouldMove = GroundSpeed > 3.f && MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector;

    bIsFalling = MovementComponent->IsFalling();

    Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, Character->GetActorRotation());
}

void ULTAnimInstance::AnimNotify_ResetMovementInput()
{
    if (ALTCharacter* LocalCharacter = Cast<ALTCharacter>(GetOwningActor()))
    {
        LocalCharacter->GetStateComponent()->ToggleMovementInput(true);
    }
}

void ULTAnimInstance::AnimNotify_ResetState()
{
    if (const ALTCharacter* LocalCharacter = Cast<ALTCharacter>(GetOwningActor()))
    {
        LocalCharacter->GetStateComponent()->ClearState();
    }
}

void ULTAnimInstance::UpdateCombatMode(const ECombatType InCombatType)
{
    CombatType = InCombatType;
}

void ULTAnimInstance::OnChangedCombat(const bool bInCombatEnabled)
{
    bCombatEnabled = bInCombatEnabled;
}
