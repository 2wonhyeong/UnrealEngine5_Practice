// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LTWeaponCollisionComponent.h"

#include "Kismet/KismetSystemLibrary.h"

ULTWeaponCollisionComponent::ULTWeaponCollisionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    //충돌을 감지할 오브젝트 타입 추가
    TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
}

void ULTWeaponCollisionComponent::BeginPlay()
{
    Super::BeginPlay();


}
//충돌을 감지
void ULTWeaponCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsCollisionEnabled)
    {
        CollisionTrace();
    }
}

void ULTWeaponCollisionComponent::TurnOnCollision()
{
    AlreadyHitActors.Empty();
    bIsCollisionEnabled = true;
}

void ULTWeaponCollisionComponent::TurnOffCollision()
{
    bIsCollisionEnabled = false;
}

void ULTWeaponCollisionComponent::SetWeaponMesh(UPrimitiveComponent* MeshComponent)
{
    WeaponMesh = MeshComponent;
}

void ULTWeaponCollisionComponent::AddIgnoredActor(AActor* Actor)
{
    IgnoredActors.Add(Actor);
}

void ULTWeaponCollisionComponent::RemoveIgnoredActor(AActor* Actor)
{
    IgnoredActors.Remove(Actor);
}

bool ULTWeaponCollisionComponent::CanHitActor(AActor* Actor) const
{
    return AlreadyHitActors.Contains(Actor) == false;
}
//충돌 처리
void ULTWeaponCollisionComponent::CollisionTrace()
{
    TArray<FHitResult> OutHits;

    const FVector Start = WeaponMesh->GetSocketLocation(TraceStartSocketName);
    const FVector End = WeaponMesh->GetSocketLocation(TraceEndSocketName);

    bool const bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
        GetOwner(),
        Start,
        End,
        TraceRadius,
        TraceObjectTypes,
        false,
        IgnoredActors,
        DrawDebugType,
        OutHits,
        true);

    if (bHit)
    {
        for (const FHitResult& Hit : OutHits)
        {
            AActor* HitActor = Hit.GetActor();

            if (HitActor == nullptr)
            {
                continue;
            }

            if (CanHitActor(HitActor))
            {
                AlreadyHitActors.Add(HitActor);

                // Call OnHitActor Broadcast
                if (OnHitActor.IsBound())
                {
                    OnHitActor.Broadcast(Hit);
                }
            }
        }
    }
}