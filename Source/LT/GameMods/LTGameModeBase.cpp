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
        // 타이머를 설정하기 위한 핸들
        FTimerHandle RespawnTimerHandle;

        // 타이머 델리게이트 생성
        FTimerDelegate RespawnDelegate;
        RespawnDelegate.BindUFunction(this, FName("RespawnPlayer"), PlayerController);

        // RespawnDelay(5초) 후에 RespawnPlayer 함수를 한 번만 실행
        GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, RespawnDelay, false);
    }
}

void ALTGameModeBase::RespawnPlayer(AController* Controller)
{
    if (Controller)
    {
        // 1. 맵에 배치된 PlayerStart 액터를 찾음
        AActor* PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
        FTransform SpawnTransform = PlayerStart ? PlayerStart->GetActorTransform() : FTransform::Identity;

        // 2. 현재 조종하고 있는 캐릭터(죽은 시체)를 파괴
        if (APawn* CurrentPawn = Controller->GetPawn())
        {
            CurrentPawn->Destroy();
        }

        // 3. 새로운 캐릭터를 스폰
        APawn* NewPawn = GetWorld()->SpawnActor<ALTCharacter>(DefaultPawnClass, SpawnTransform);

        // 4. 컨트롤러가 새로 스폰된 캐릭터에 빙의(조종 시작)
        if (NewPawn)
        {
            Controller->Possess(NewPawn);
        }
    }
}
