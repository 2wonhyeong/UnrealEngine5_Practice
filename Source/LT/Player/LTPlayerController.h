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
	// 현재 화면에 표시되고 있는 사망 위젯을 저장할 포인터
	UPROPERTY()
	UUserWidget* DeathWidget;

public:
	// 캐릭터가 이 함수를 호출하여 사망 위젯 표시를 요청합니다.
	void ShowDeathWidget(TSubclassOf<UUserWidget> NewDeathWidgetClass);

protected:
	// 새로운 캐릭터에 빙의할 때마다 호출
	virtual void OnPossess(APawn* InPawn) override;

	// 사망 위젯을 화면에서 제거하는 내부 함수
	void HideDeathWidget();
};
