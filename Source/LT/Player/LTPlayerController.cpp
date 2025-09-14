// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LTPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/LTPlayerHUDWidget.h"
#include "Character/LTCharacter.h"
#include "Components/LTAttributeComponent.h"

void ALTPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 부활했으므로, 화면에 남아있던 사망 UI를 제거합니다.
	HideDeathWidget();
}

void ALTPlayerController::ShowDeathWidget(TSubclassOf<UUserWidget> NewDeathWidgetClass)
{
	// 혹시 모를 중복 생성을 방지
	if (DeathWidget)
	{
		return;
	}

	if (NewDeathWidgetClass)
	{
		// 위젯을 생성하고, 포인터를 저장해 둡니다.
		DeathWidget = CreateWidget<UUserWidget>(this, NewDeathWidgetClass);
		if (DeathWidget)
		{
			DeathWidget->AddToViewport();
		}
	}
}

void ALTPlayerController::HideDeathWidget()
{
	if (DeathWidget)
	{
		DeathWidget->RemoveFromParent();
		DeathWidget = nullptr;
	}
}

