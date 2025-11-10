// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LTPotionWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void ULTPotionWidget::SetPotionQuantity(const int InAmount) const
{
	PotionQuantityText->SetText(UKismetTextLibrary::Conv_IntToText(InAmount));
}
