// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DS1WeaponCollisionComponent.generated.h"

//무기에 어떤 물체가 충돌했을 때 충돌 결과를 처리해주는 DELEGATE
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitActor, const FHitResult&);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), meta = (DisplayName = "Weapon Collision"))
class DS1_API UDS1WeaponCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnHitActor OnHitActor;

protected:
	UPROPERTY(EditAnywhere)
	FName TraceStartSocketName;
	UPROPERTY(EditAnywhere)
	FName TraceEndSocketName;

protected://충돌처리를 할 때 필요한 프로퍼티
	//원 크기
	UPROPERTY(EditAnywhere)
	float TraceRadius = 20.f;
	//어떤 오프젝트타입을 충돌처리할건지 알려주는 배열
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	//충돌처리 제외 액터
	UPROPERTY(EditAnywhere)
	TArray<AActor*> IgnoredActors;
	//충돌처리가 제대로 됐는지 확인하는 디버그 드로우 타입
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;

protected:
	//무기의 메시컴포넌트 ( ex:스태틱메시, 스켈레탈메시 )
	//상위 클래스의 타입으로 정의해놓음
	UPROPERTY()
	UPrimitiveComponent* WeaponMesh;
	//충돌 여러번 안되게
	UPROPERTY()
	TArray<AActor*> AlreadyHitActors;
	//충돌처리를 끄고 킬 수 있도록
	bool bIsCollisionEnabled = false;

public:	
	// Sets default values for this component's properties
	UDS1WeaponCollisionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public://변수들의 값을 변경시켜주는 함수들
	void TurnOnCollision();
	void TurnOffCollision();
	void SetWeaponMesh(UPrimitiveComponent* MeshComponent);
	void AddIgnoredActor(AActor* Actor);
	void RemoveIgnoredActor(AActor* Actor);
		
protected:
	bool CanHitActor(AActor* Actor) const;
	void CollisionTrace();
};
