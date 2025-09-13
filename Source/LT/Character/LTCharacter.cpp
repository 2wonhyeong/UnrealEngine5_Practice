// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LTCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/LTAttributeComponent.h"
#include "Components/LTStateComponent.h"
#include "Components/LTTargetingComponent.h"
#include "LTGamePlayTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Equipments/LTFistWeapon.h"
#include "Components/LTCombatComponent.h"
#include "Interfaces/LTInteract.h"
#include "Engine/DamageEvents.h"

// Sets default values
ALTCharacter::ALTCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

	//기본속도
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	AttributeComponent = CreateDefaultSubobject<ULTAttributeComponent>(TEXT("Attribute"));
	StateComponent = CreateDefaultSubobject<ULTStateComponent>(TEXT("State"));

	CombatComponent = CreateDefaultSubobject<ULTCombatComponent>(TEXT("Combat"));

	TargetingComponent = CreateDefaultSubobject<ULTTargetingComponent>(TEXT("Targeting"));
}

// Called when the game starts or when spawned
void ALTCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerHUDWidgetClass)
	{
		PlayerHUDWidget = CreateWidget<ULTPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
		{
			PlayerHUDWidget->AddToViewport();
		}
	}
	if (FistWeaponClass)
	{
		FActorSpawnParameters Spawnparams;
		Spawnparams.Owner = this;
		ALTFistWeapon* FistWeapon = GetWorld()->SpawnActor<ALTFistWeapon>(FistWeaponClass, GetActorTransform(), Spawnparams);
		FistWeapon->EquipItem();
	}
}

// Called every frame
void ALTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Cyan, FString::Printf(TEXT("Stamina : %.1f"), AttributeComponent->GetBaseStamina()));
	//GEngine->AddOnScreenDebugMessage(2, 1.5f, FColor::Cyan, FString::Printf(TEXT("MaxWalkSpeed : %.1f"), GetCharacterMovement()->MaxWalkSpeed));
}

void ALTCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called to bind functionality to input
void ALTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALTCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALTCharacter::Look);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ALTCharacter::Sprinting);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ALTCharacter::StopSprint);

		EnhancedInputComponent->BindAction(RollingAction, ETriggerEvent::Triggered, this, &ALTCharacter::Rolling);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ALTCharacter::Interact);

		EnhancedInputComponent->BindAction(ToggleCombatAction, ETriggerEvent::Started, this, &ALTCharacter::ToggleCombat);

		EnhancedInputComponent->BindAction(TestDamageAction, ETriggerEvent::Started, this, &ALTCharacter::ApplyDamage);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ALTCharacter::AutoToggleCombat);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Canceled, this, &ALTCharacter::Attack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ALTCharacter::SpecialAttack);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &ALTCharacter::HeavyAttack);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ALTCharacter::Jumping);

		EnhancedInputComponent->BindAction(LockOnTargetAction, ETriggerEvent::Started, this, &ALTCharacter::LockOnTarget);
		EnhancedInputComponent->BindAction(LeftTargetActiron, ETriggerEvent::Started, this, &ALTCharacter::LeftTarget);
		EnhancedInputComponent->BindAction(RightTargetAction, ETriggerEvent::Started, this, &ALTCharacter::RightTarget);
	}
}
float ALTCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		// AttributeComponent의 ApplyHealthChange 함수를 호출하여 체력을 감소시킵니다. (음수 값 전달)
		AttributeComponent->ApplyHealthChange(-ActualDamage);
	}
	return ActualDamage;
}
void ALTCharacter::Move(const FInputActionValue& Values)
{
	check(StateComponent);
	if (StateComponent->MovementInputEnabled() == false)
	{
		return;
	}

	FVector2D MovementVector = Values.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotator = Controller->GetControlRotation();
		const FRotator YawRotator(0, Rotator.Yaw, 0);

		const FVector ForwardVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
		const FVector RightVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardVector, MovementVector.Y);
		AddMovementInput(RightVector, MovementVector.X);
	}
}
void ALTCharacter::Look(const FInputActionValue& Values)
{
	if (TargetingComponent && TargetingComponent->IsLockOn())
		return;

	FVector2D LookDirection = Values.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookDirection.X);
		AddControllerPitchInput(LookDirection.Y);
	}
}
bool ALTCharacter::IsMoving() const
{
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		//캐릭터의 속도가 3이상이고 가속중이면 이동중으로 판단
		return MovementComp->Velocity.Size2D() > 3.f && MovementComp->GetCurrentAcceleration() != FVector::Zero();
	}
	return 0;
}
void ALTCharacter::Rolling()
{
	check(AttributeComponent);
	check(StateComponent);

	if (AttributeComponent->CheckHasEnoughStamina(10.f)&&CanPerformRoll())
	{
		AttributeComponent->ToggleStaminaRegeneration(false);

		StateComponent->ToggleMovementInput(false);

		AttributeComponent->DecreaseStamina(10.f);

		PlayAnimMontage(RollingMontage);

		StateComponent->SetState(LTGamePlayTags::Character_State_Rolling);

		AttributeComponent->ToggleStaminaRegeneration(true, 1.5f);
	}
}
bool ALTCharacter::CanPerformRoll() const
{
	if (StateComponent)
	{
		FGameplayTagContainer UninterruptibleTags;
		UninterruptibleTags.AddTag(LTGamePlayTags::Character_State_Jumping);
		UninterruptibleTags.AddTag(LTGamePlayTags::Character_State_Rolling);

		if (StateComponent->IsCurrentStateEqualToAny(UninterruptibleTags))
		{
			return false;
		}
	}
	return true;
}
void ALTCharacter::Sprinting()
{
	if (AttributeComponent->CheckHasEnoughStamina(0.1f) && IsMoving())
	{
		AttributeComponent->ToggleStaminaRegeneration(false);

		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;

		AttributeComponent->DecreaseStamina(0.1f);

		bSprinting = true;
	}
	else
		StopSprint();
}
void ALTCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	AttributeComponent->ToggleStaminaRegeneration(true);
	bSprinting = false;
}
void ALTCharacter::Interact()
{
	FHitResult OutHit;
	const FVector Start = GetActorLocation();
	const FVector End = Start;
	const FVector BoxHalfSize = FVector(75.f, 75.f, 100.f);
	const FRotator Orientation = GetActorRotation();
	constexpr float Radius = 100.f;

	TArray<TEnumAsByte<EObjectTypeQuery>>ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_OBJECT_INTERACTION));

	TArray<AActor*> ActorToIgnore;

	bool bHit = UKismetSystemLibrary::BoxTraceSingleForObjects(//구 형태의 감지영역을 만들어서 그 안에 들어온 물체 감지
		this,//월드 컨텍스트 오브젝트
		Start,//감지영역의 시작부분
		End,//감지영역의 끝부분
		BoxHalfSize,//감지영역의 반경
		Orientation,
		ObjectTypes,//어떤 형태의 오브젝트를 감지할지(WorldStatic)
		false,//bTraceComplex 복잡한 콜리전 검사
		ActorToIgnore,//감지영역에 들어온 것 중 제외 할 것들
		EDrawDebugTrace::ForDuration,//제대로 영역이 생성이 되었는지 확인
		OutHit,//결과값
		true//bIgnoreSelf
	);

	if (bHit)
	{
		if (AActor* HitActor = OutHit.GetActor())
		{
			if (ILTInteract* Interaction = Cast<ILTInteract>(HitActor))
			{
				Interaction->Interact(this);
			}
		}
	}
}
bool ALTCharacter::CanToggleCombat() const
{
	check(StateComponent);

	if (IsValid(CombatComponent->GetMainWeapon()) == false)
		return false;
	if (CombatComponent->GetMainWeapon()->GetCombatType() == ECombatType::MeleeFists)
		return false;

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(LTGamePlayTags::Character_State_Attacking);
	CheckTags.AddTag(LTGamePlayTags::Character_State_Rolling);
	CheckTags.AddTag(LTGamePlayTags::Character_State_GeneralAction);

	return StateComponent->IsCurrentStateEqualToAny(CheckTags) == false;
}
void ALTCharacter::ToggleCombat()
{
	check(CombatComponent);
	check(StateComponent);

	if(CombatComponent)
	{
		if (const ALTWeapon* Weapon = CombatComponent->GetMainWeapon())
		{
			if (CanToggleCombat())
			{
				StateComponent->SetState(LTGamePlayTags::Character_State_GeneralAction);
				if (CombatComponent->IsCombatEnabled())
				{
					PlayAnimMontage(Weapon->GetMontageForTag(LTGamePlayTags::Character_Action_Unequip));
				}
				else 
				{
					PlayAnimMontage(Weapon->GetMontageForTag(LTGamePlayTags::Character_Action_Equip));
				}
			}
		}
	}
}
void ALTCharacter::AutoToggleCombat()
{
	if (CombatComponent)
	{
		if (!CombatComponent->IsCombatEnabled())
		{
			ToggleCombat();
		}
	}
}
void ALTCharacter::Attack()
{
	const FGameplayTag AttackTypeTag = GetAttackPerform();

	if (CanPerformAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}
void ALTCharacter::SpecialAttack()
{
	const FGameplayTag AttackTypeTag = LTGamePlayTags::Character_Attack_Special;

	if (CanPerformAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}
void ALTCharacter::HeavyAttack()
{
	AutoToggleCombat();
	const FGameplayTag AttackTypeTag = LTGamePlayTags::Character_Attack_Heavy;

	if (CanPerformAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}
void ALTCharacter::ApplyDamage()
{
	TakeDamage(10.f, FDamageEvent(), GetController(), this);
}
FGameplayTag ALTCharacter::GetAttackPerform() const
{
	if (IsSprinting())
	{
		return LTGamePlayTags::Character_Attack_Running;
	}
	return LTGamePlayTags::Character_Attack_Light;
}
void ALTCharacter::DoAttack(const FGameplayTag& AttackTypeTag)
{
	check(StateComponent);
	check(CombatComponent);
	check(AttributeComponent);

	if (const ALTWeapon* Weapon = CombatComponent->GetMainWeapon())
	{
		StateComponent->SetState(LTGamePlayTags::Character_State_Attacking);
		StateComponent->ToggleMovementInput(false);
		CombatComponent->SetLastAttackType(AttackTypeTag);

		AttributeComponent->ToggleStaminaRegeneration(false);

		UAnimMontage* Montage = Weapon->GetMontageForTag(AttackTypeTag, ComboCounter);
		if (!Montage)
		{
			ComboCounter = 0;
			Montage = Weapon->GetMontageForTag(AttackTypeTag, ComboCounter);
		}

		PlayAnimMontage(Montage);
		const float StaminaCost = Weapon->GetStaminaCost(AttackTypeTag);
		AttributeComponent->DecreaseStamina(StaminaCost);
		AttributeComponent->ToggleStaminaRegeneration(true, 1.f);
	}
}
bool ALTCharacter::CanPerformAttack(const FGameplayTag& AttackTypeTag) const
{
	check(StateComponent)
	check(CombatComponent)
	check(AttributeComponent)

	if (IsValid(CombatComponent->GetMainWeapon()) == false)
		return false;

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(LTGamePlayTags::Character_State_Rolling);
	CheckTags.AddTag(LTGamePlayTags::Character_State_GeneralAction);

	const float StaminaCost = CombatComponent->GetMainWeapon()->GetStaminaCost(AttackTypeTag);

	return StateComponent->IsCurrentStateEqualToAny(CheckTags) == false
		&& CombatComponent->IsCombatEnabled()
		&& AttributeComponent->CheckHasEnoughStamina(StaminaCost);
}
void ALTCharacter::ExecuteComboAttack(const FGameplayTag& AttackTypeTag)
{
	if (StateComponent->GetCurrentState() != LTGamePlayTags::Character_State_Attacking)
	{
		if (bComboSequenceRunning && bCanComboInput == false)
		{
			ComboCounter++;
			UE_LOG(LogTemp, Warning, TEXT("Additional input : Combo Counter = %d"), ComboCounter);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT(">>> ComboSequence Started <<<"));
			ResetCombo();
			bComboSequenceRunning = true;
		}
		DoAttack(AttackTypeTag);
		GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
	}
	else if (bCanComboInput)
	{
		bSavedComboInput = true;
	}
}
void ALTCharacter::ResetCombo()
{
	UE_LOG(LogTemp, Warning, TEXT("Combo Reset"));

	bComboSequenceRunning = false;
	bCanComboInput = false;
	bSavedComboInput = false;
	ComboCounter = 0;
}
void ALTCharacter::EnableComboWindow()
{
	bCanComboInput = true;
	UE_LOG(LogTemp, Warning, TEXT("Combo Window Opened : Combo Counter = %d"), ComboCounter);
}
void ALTCharacter::DisableComboWindow()
{
	check(CombatComponent)

	bCanComboInput = false;

	if (bSavedComboInput)
	{
		bSavedComboInput = false;
		ComboCounter++;
		UE_LOG(LogTemp, Warning, TEXT("Combo Window Opened : Advancing to next combo = %d"), ComboCounter);
		DoAttack(CombatComponent->GetLastAttackType());
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Combo Window Closed : No Input Received"));
}
void ALTCharacter::AttackFinished(const float ComboResetDelay)
{
	UE_LOG(LogTemp, Warning, TEXT("Attack Finished"));
	if (StateComponent)
	{
		StateComponent->ToggleMovementInput(true);
	}
	GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &ALTCharacter::ResetCombo, ComboResetDelay, false);

}
bool ALTCharacter::CanPerformJump() const
{
	// ACharacter에 내장된 점프 가능 여부 확인 (공중에 떠있는지 등)
	// 이 결과가 false이면 다른 것을 확인할 필요도 없음
	if (!Super::CanJump())
	{
		return false;
	}

	// StateComponent를 통해 현재 구르기, 공격 등 다른 행동 중인지 확인
	if (StateComponent)
	{
		FGameplayTagContainer UninterruptibleTags;
		UninterruptibleTags.AddTag(LTGamePlayTags::Character_State_Attacking);
		UninterruptibleTags.AddTag(LTGamePlayTags::Character_State_Rolling);

		if (StateComponent->IsCurrentStateEqualToAny(UninterruptibleTags))
		{
			return false;
		}
	}
	// 위 모든 조건을 통과했다면 점프 가능
	return true;
}
void ALTCharacter::Jumping()
{
	// 1. 점프가 가능한 상태인지 먼저 확인
	if (CanPerformJump())
	{
		StateComponent->SetState(LTGamePlayTags::Character_State_Jumping);
		Super::Jump();
	}
}
void ALTCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit); // 부모 클래스의 기능을 먼저 호출

	if (StateComponent)
	{
		// 만약 현재 상태가 'Jumping'이었다면, 상태를 초기화합니다.
		if (StateComponent->GetCurrentState() == LTGamePlayTags::Character_State_Jumping)
		{
			StateComponent->ClearState();
		}
	}
}
void ALTCharacter::LockOnTarget()
{
	TargetingComponent->ToggleLockOn();
}
void ALTCharacter::LeftTarget()
{
	TargetingComponent->SwitchingLockedOnActor(ESwitchingDirection::Left);
}
void ALTCharacter::RightTarget()
{
	TargetingComponent->SwitchingLockedOnActor(ESwitchingDirection::Right);
}
void ALTCharacter::ActivateWeaponCollision(EWeaponCollisionType WeaponCollisionType)
{
	if (CombatComponent)
		CombatComponent->GetMainWeapon()->ActivateCollision(WeaponCollisionType);
}
void ALTCharacter::DeactivateWeaponCollision(EWeaponCollisionType WeaponCollisionType)
{
	if (CombatComponent)
		CombatComponent->GetMainWeapon()->DeactivateCollision(WeaponCollisionType);
}