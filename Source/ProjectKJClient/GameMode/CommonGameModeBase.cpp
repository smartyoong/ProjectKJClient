// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonGameModeBase.h"

/*
void AMyGameMode::SpawnPlayerCharacter()
{
    if (GetWorld())
    {
        // 소환할 캐릭터 클래스 지정
        TSubclassOf<APawn> PlayerPawnClass = AMyPlayerCharacter::StaticClass();

        // 소환 위치와 회전값 설정
        FVector SpawnLocation = FVector(100.0f, 100.0f, 100.0f); // 예시 위치
        FRotator SpawnRotation = FRotator::ZeroRotator; // 회전 없음

        // 캐릭터 소환
        APawn* PlayerPawn = GetWorld()->SpawnActor<APawn>(PlayerPawnClass, SpawnLocation, SpawnRotation);

        if (PlayerPawn)
        {
            // 게임에 참여 중인 첫 번째 플레이어 컨트롤러 가져오기
            APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

            if (PlayerController)
            {
                // 플레이어 컨트롤러로 캐릭터 제어하기
                PlayerController->Possess(PlayerPawn);
            }
        }
    }
}
*/