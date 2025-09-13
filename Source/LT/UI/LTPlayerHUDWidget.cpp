// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LTPlayerHUDWidget.h"
#include "StatBarWidget.h"
#include "UI/My_HPBarWidget.h"
#include "Components/LTAttributeComponent.h"

ULTPlayerHUDWidget::ULTPlayerHUDWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}
void ULTPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		if (ULTAttributeComponent* Attribute = OwningPawn->GetComponentByClass<ULTAttributeComponent>())
		{
			Attribute->OnAttributeChanged.AddUObject(this, &ULTPlayerHUDWidget::OnAttributeChanged);
			Attribute->BroadcastAttributeChanged(ELTAttributeType::Stamina);
			Attribute->BroadcastAttributeChanged(ELTAttributeType::Health);
		}
	}
}

void ULTPlayerHUDWidget::OnAttributeChanged(ELTAttributeType AttributeType, float InValue)
{
	switch (AttributeType)
	{
	case ELTAttributeType::Stamina:
		StaminaBarWidget->SetRatio(InValue);
		break;
	case ELTAttributeType::Health:
		HPBarWidget->SetRatio(InValue);
		break;
	}
}