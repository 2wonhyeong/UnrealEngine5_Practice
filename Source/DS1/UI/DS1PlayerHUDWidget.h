// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DS1Define.h"
#include "Blueprint/UserWidget.h"
#include "DS1PlayerHUDWidget.generated.h"

class UDS1StatBarWidget;
class UMy_HPBarWidget;
/**
 * 
 */
UCLASS()
class DS1_API UDS1PlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UDS1StatBarWidget* StaminaBarWidget;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UMy_HPBarWidget* HPBarWidget;
	
public:
	UDS1PlayerHUDWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void NativeConstruct() override;

protected:
	void OnAttributeChanged(EDS1AttributeType AttributeType, float InValue);
};
