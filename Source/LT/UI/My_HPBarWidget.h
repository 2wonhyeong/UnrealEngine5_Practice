// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "My_HPBarWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class LT_API UMy_HPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UProgressBar* HPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatBar")
	FLinearColor FillColorAndOpacity = FLinearColor::Red;

public:
	UMy_HPBarWidget(const FObjectInitializer& ObjectInitializer);

public:
	virtual void NativePreConstruct() override;

public:
	void SetRatio(float Ratio) const;
};
