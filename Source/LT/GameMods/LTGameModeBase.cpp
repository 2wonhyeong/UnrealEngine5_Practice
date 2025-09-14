// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMods/LTGameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Character/LTCharacter.h"
#include "UI/LTPlayerHUDWidget.h"

void ALTGameModeBase::PlayerDied(APlayerController* PlayerController)
{
    if (PlayerController)
    {
        // Ÿ�̸Ӹ� �����ϱ� ���� �ڵ�
        FTimerHandle RespawnTimerHandle;

        // Ÿ�̸� ��������Ʈ ����
        FTimerDelegate RespawnDelegate;
        RespawnDelegate.BindUFunction(this, FName("RespawnPlayer"), PlayerController);

        // RespawnDelay(5��) �Ŀ� RespawnPlayer �Լ��� �� ���� ����
        GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, RespawnDelay, false);
    }
}

void ALTGameModeBase::RespawnPlayer(AController* Controller)
{
    if (Controller)
    {
        // 1. �ʿ� ��ġ�� PlayerStart ���͸� ã��
        AActor* PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
        FTransform SpawnTransform = PlayerStart ? PlayerStart->GetActorTransform() : FTransform::Identity;

        // 2. ���� �����ϰ� �ִ� ĳ����(���� ��ü)�� �ı�
        if (APawn* CurrentPawn = Controller->GetPawn())
        {
            CurrentPawn->Destroy();
        }

        // 3. ���ο� ĳ���͸� ����
        APawn* NewPawn = GetWorld()->SpawnActor<ALTCharacter>(DefaultPawnClass, SpawnTransform);

        // 4. ��Ʈ�ѷ��� ���� ������ ĳ���Ϳ� ����(���� ����)
        if (NewPawn)
        {
            Controller->Possess(NewPawn);
        }
    }
}
