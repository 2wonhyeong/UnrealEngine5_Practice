// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DS1WeaponCollisionComponent.generated.h"

//���⿡ � ��ü�� �浹���� �� �浹 ����� ó�����ִ� DELEGATE
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

protected://�浹ó���� �� �� �ʿ��� ������Ƽ
	//�� ũ��
	UPROPERTY(EditAnywhere)
	float TraceRadius = 20.f;
	//� ������ƮŸ���� �浹ó���Ұ��� �˷��ִ� �迭
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	//�浹ó�� ���� ����
	UPROPERTY(EditAnywhere)
	TArray<AActor*> IgnoredActors;
	//�浹ó���� ����� �ƴ��� Ȯ���ϴ� ����� ��ο� Ÿ��
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;

protected:
	//������ �޽�������Ʈ ( ex:����ƽ�޽�, ���̷�Ż�޽� )
	//���� Ŭ������ Ÿ������ �����س���
	UPROPERTY()
	UPrimitiveComponent* WeaponMesh;
	//�浹 ������ �ȵǰ�
	UPROPERTY()
	TArray<AActor*> AlreadyHitActors;
	//�浹ó���� ���� ų �� �ֵ���
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

public://�������� ���� ��������ִ� �Լ���
	void TurnOnCollision();
	void TurnOffCollision();
	void SetWeaponMesh(UPrimitiveComponent* MeshComponent);
	void AddIgnoredActor(AActor* Actor);
	void RemoveIgnoredActor(AActor* Actor);
		
protected:
	bool CanHitActor(AActor* Actor) const;
	void CollisionTrace();
};
