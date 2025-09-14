// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LTPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/LTPlayerHUDWidget.h"
#include "Character/LTCharacter.h"
#include "Components/LTAttributeComponent.h"

void ALTPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// ��Ȱ�����Ƿ�, ȭ�鿡 �����ִ� ��� UI�� �����մϴ�.
	HideDeathWidget();
}

void ALTPlayerController::ShowDeathWidget(TSubclassOf<UUserWidget> NewDeathWidgetClass)
{
	// Ȥ�� �� �ߺ� ������ ����
	if (DeathWidget)
	{
		return;
	}

	if (NewDeathWidgetClass)
	{
		// ������ �����ϰ�, �����͸� ������ �Ӵϴ�.
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

