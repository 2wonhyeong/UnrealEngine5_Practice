// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LTDefine.h"
#include "Blueprint/UserWidget.h"
#include "LTPlayerHUDWidget.generated.h"

class UStatBarWidget;
class UMy_HPBarWidget;
/**
 * 
 */
UCLASS()
class LT_API ULTPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UStatBarWidget* StaminaBarWidget;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UMy_HPBarWidget* HPBarWidget;
	
public:
	ULTPlayerHUDWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void NativeConstruct() override;

protected:
	void OnAttributeChanged(ELTAttributeType AttributeType, float InValue);
};
