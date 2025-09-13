// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/My_HPBarWidget.h"
#include "Components/ProgressBar.h"

UMy_HPBarWidget::UMy_HPBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// �ʿ��� �ʱ�ȭ �ڵ�� ���⿡ �ۼ��մϴ�.
}

void UMy_HPBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (HPBar)
	{
		HPBar->SetFillColorAndOpacity(FillColorAndOpacity);
	}
}
void UMy_HPBarWidget::SetRatio(float Ratio) const
{
	if (HPBar)
	{
		HPBar->SetPercent(Ratio);
	}
}
