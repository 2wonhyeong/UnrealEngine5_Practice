// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LTPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LT_API ALTPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	// ���� ȭ�鿡 ǥ�õǰ� �ִ� ��� ������ ������ ������
	UPROPERTY()
	UUserWidget* DeathWidget;

public:
	// ĳ���Ͱ� �� �Լ��� ȣ���Ͽ� ��� ���� ǥ�ø� ��û�մϴ�.
	void ShowDeathWidget(TSubclassOf<UUserWidget> NewDeathWidgetClass);

protected:
	// ���ο� ĳ���Ϳ� ������ ������ ȣ��
	virtual void OnPossess(APawn* InPawn) override;

	// ��� ������ ȭ�鿡�� �����ϴ� ���� �Լ�
	void HideDeathWidget();
};
