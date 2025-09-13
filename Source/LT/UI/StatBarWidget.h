// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatBarWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class LT_API UStatBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget),BlueprintReadWrite)
	UProgressBar* StatBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StatBar")
	FLinearColor FillColorAndOpacity = FLinearColor::Blue;
	
public:
	UStatBarWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void NativePreConstruct() override;

public:
	void SetRatio(float Ratio) const;
};
