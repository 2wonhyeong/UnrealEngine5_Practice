// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LTPotionWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class LT_API ULTPotionWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget),BlueprintReadWrite)
	UTextBlock* PotionQuantityText;

public:
	void SetPotionQuantity(const int InAmount) const;
};
