// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UI/DS1PlayerHUDWidget.h"
#include "DS1Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UDS1AttributeComponent;

UCLASS()
class DS1_API ADS1Character : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SprintAction;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UDS1AttributeComponent* AttributeComponent;

protected:
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;
	UPROPERTY()
	UDS1PlayerHUDWidget* PlayerHUDWidget;
	

protected:
	//질주속도
	UPROPERTY(EditAnywhere, Category="Sprinting")
	float SprintingSpeed = 750.f;
	//기본속도
	UPROPERTY(EditAnywhere, Category = "Sprinting")
	float NormalSpeed = 500.f;

public:
	// Sets default values for this character's properties
	ADS1Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UFUNCTION()
	void Move(const FInputActionValue& Values);
	UFUNCTION()
	void Look(const FInputActionValue& Values);
	UFUNCTION()
	void Sprinting();
	UFUNCTION()
	void StopSprint();
	UFUNCTION()
	bool IsMoving() const;
};
