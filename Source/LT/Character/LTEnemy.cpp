// Fill out your copyright notice in the Description page of Project Settings.


#include "LTEnemy.h"

#include "LTDefine.h"
#include "GameplayTagContainer.h"
#include "Components/CapsuleComponent.h"
#include "Components/LTAttributeComponent.h"
#include "Components/LTStateComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "LTGameplayTags.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Equipments/LTWeapon.h"
#include "UI/My_HPBarWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISense_Damage.h"
#include "AIController.h"
#include "BrainComponent.h"

ALTEnemy::ALTEnemy()
{
	//?
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// Targeting 구체 생성및 Collision 설정.
	TargetingSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("TargetingSphere"));
	TargetingSphereComponent->SetupAttachment(GetRootComponent());
	TargetingSphereComponent->SetCollisionObjectType(COLLISION_OBJECT_TARGETING);
	TargetingSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	TargetingSphereComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	// LockOn 위젯.
	LockOnWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidgetComponent"));
	LockOnWidgetComponent->SetupAttachment(GetRootComponent());
	LockOnWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	LockOnWidgetComponent->SetDrawSize(FVector2D(30.f, 30.f));
	LockOnWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	LockOnWidgetComponent->SetVisibility(false);

	HPBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidgetComponent"));
	HPBarWidgetComponent->SetupAttachment(GetRootComponent());
	HPBarWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	HPBarWidgetComponent->SetDrawSize(FVector2D(100.f, 10.f));
	HPBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HPBarWidgetComponent->SetVisibility(false);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	AttributeComponent = CreateDefaultSubobject<ULTAttributeComponent>(TEXT("Attribute"));
	StateComponent = CreateDefaultSubobject<ULTStateComponent>("State");
	CombatComponent = CreateDefaultSubobject<ULTCombatComponent>("Combat");

	// OnDeath Delegate에 함수 바인딩.
	AttributeComponent->OnDeath.AddUObject(this, &ALTEnemy::OnDeath);
	AttributeComponent->OnAttributeChanged.AddUObject(this, &ALTEnemy::OnAttributeChanged);
}

void ALTEnemy::BeginPlay()
{
	Super::BeginPlay();

	//무기 장착
	if (DefaultWeaponClass)
	{
		FActorSpawnParameters Params;
		Params.Owner = this;

		ALTWeapon* Weapon = GetWorld()->SpawnActor<ALTWeapon>(DefaultWeaponClass, GetActorTransform(), Params);
		CombatComponent->SetCombatEnabled(true);
		Weapon->EquipItem();
	}
	SetupHpBar();
}

void ALTEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ALTEnemy::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float  ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (AttributeComponent)
	{
		AttributeComponent->TakeDamageAmount(ActualDamage);
		GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Cyan, FString::Printf(TEXT("Damaged : %f"), ActualDamage));
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);

		// 데미지 방향
		FVector ShotDirection = PointDamageEvent->ShotDirection;
		// 히트 위치 (표면 접촉 관점)
		FVector ImpactPoint = PointDamageEvent->HitInfo.ImpactPoint;
		// 히트 방향
		FVector ImpactDirection = PointDamageEvent->HitInfo.ImpactNormal;
		// 히트한 객체의 Location (객체 중심 관점)
		FVector HitLocation = PointDamageEvent->HitInfo.Location;
		//데미지를 받았다고 알려주는 함수
		UAISense_Damage::ReportDamageEvent(GetWorld(), this, EventInstigator->GetPawn(), ActualDamage, HitLocation, HitLocation);

		ImpactEffect(ImpactPoint);

		HitReaction(EventInstigator->GetPawn());
	}

	return ActualDamage;
}

void ALTEnemy::OnDeath()
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
		AIController->GetBrainComponent()->StopLogic(TEXT("Death"));
	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Ragdoll
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		MeshComp->SetCollisionProfileName("Ragdoll");
		MeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		MeshComp->SetSimulatePhysics(true);
		ToggleHPBarVisibility(false);
	}
}

void ALTEnemy::ImpactEffect(const FVector& Location)
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Location);
	}

	if (ImpactParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, Location);
	}
}

void ALTEnemy::HitReaction(const AActor* Attacker)
{
	check(CombatComponent);

	if (UAnimMontage* HitReactAnimMontage = CombatComponent->GetMainWeapon()->GetHitReactMontage(Attacker))
	{
		PlayAnimMontage(HitReactAnimMontage);
	}
}

void ALTEnemy::OnTargeted(bool bTargeted)
{
	if (LockOnWidgetComponent)
	{
		LockOnWidgetComponent->SetVisibility(bTargeted);
	}
}

bool ALTEnemy::CanBeTargeted()
{
	if (!StateComponent)
		return false;

	FGameplayTagContainer TagCheck;
	TagCheck.AddTag(LTGamePlayTags::Character_State_Death);
	return StateComponent->IsCurrentStateEqualToAny(TagCheck) == false;
}

void ALTEnemy::ActivateWeaponCollision(EWeaponCollisionType WeaponCollisionType)
{
	if (CombatComponent)
		CombatComponent->GetMainWeapon()->ActivateCollision(WeaponCollisionType);
}

void ALTEnemy::DeactivateWeaponCollision(EWeaponCollisionType WeaponCollisionType)
{
	if (CombatComponent)
		CombatComponent->GetMainWeapon()->DeactivateCollision(WeaponCollisionType);
}

void ALTEnemy::PerformAttack(FGameplayTag& AttackTypeTag, FOnMontageEnded& MontageEndedDelegate)
{
	check(StateComponent);
	check(CombatComponent);

	if (const ALTWeapon* Weapon = CombatComponent->GetMainWeapon())
	{
		StateComponent->SetState(LTGamePlayTags::Character_State_Attacking);
		CombatComponent->SetLastAttackType(AttackTypeTag);

		UAnimMontage* Montage = Weapon->GetRandomMontageForTag(AttackTypeTag);
		if (Montage)
		{
			if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
			{
				AnimInstance->Montage_Play(Montage);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, Montage);
			}
		}
	}
}
void ALTEnemy::ToggleHPBarVisibility(bool bVisibility)
{
	if (HPBarWidgetComponent)
		HPBarWidgetComponent->SetVisibility(bVisibility);
}
//StatBar말고 HPBar로 캐스트해도 되나? 내 코드에 있는 HPBar는 필요없는 파일인가?
void ALTEnemy::OnAttributeChanged(ELTAttributeType AttributeType, float InValue)
{
	if (AttributeType == ELTAttributeType::Health)
	{
		if (HPBarWidgetComponent)
		{
			if (const UMy_HPBarWidget* HPBar = Cast<UMy_HPBarWidget>(HPBarWidgetComponent->GetWidget()))
				HPBar->SetRatio(InValue);
		}
	}
}

void ALTEnemy::SetupHpBar()
{
	if (AttributeComponent)
	{
		AttributeComponent->BroadcastAttributeChanged(ELTAttributeType::Health);
	}
}

/*UAnimMontage* ALTEnemy::GetHitReactAnimation(const AActor* Attacker) const
{
	// LookAt 회전값을 구합니다. (현재 Actor가 공격자를 바라보는 회전값)
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Attacker->GetActorLocation());
	// 현재 Actor의 회전값과 LookAt 회전값의 차이를 구합니다.
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), LookAtRotation);
	// Z축 기준의 회전값 차이만을 취합니다.
	const float DeltaZ = DeltaRotation.Yaw;

	EHitDirection HitDirection = EHitDirection::Front;

	if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -45.f, 45.f))
	{
		HitDirection = EHitDirection::Front;
		UE_LOG(LogTemp, Log, TEXT("Front"));
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 45.f, 135.f))
	{
		HitDirection = EHitDirection::Left;
		UE_LOG(LogTemp, Log, TEXT("Left"));
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 135.f, 180.f)
		|| UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -180.f, -135.f))
	{
		HitDirection = EHitDirection::Back;
		UE_LOG(LogTemp, Log, TEXT("Back"));
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -135.f, -45.f))
	{
		HitDirection = EHitDirection::Right;
		UE_LOG(LogTemp, Log, TEXT("Right"));
	}

	UAnimMontage* SelectedMontage = nullptr;
	switch (HitDirection)
	{
	case EHitDirection::Front:
		SelectedMontage = HitReactAnimFront;
		break;
	case EHitDirection::Back:
		SelectedMontage = HitReactAnimBack;
		break;
	case EHitDirection::Left:
		SelectedMontage = HitReactAnimLeft;
		break;
	case EHitDirection::Right:
		SelectedMontage = HitReactAnimRight;
		break;
	}

	return SelectedMontage;
}*/