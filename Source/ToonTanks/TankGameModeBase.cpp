// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"
#include "Pawn_Tank.h"
#include "PawnTurret.h"
#include "ToonTanks/Controllers/PlayerControllerBase.h"
#include "Kismet/GameplayStatics.h"

void ATankGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    HandleGameStart();
}

void ATankGameModeBase::HandleGameStart()
{
    TargetTurrets = GetTargetTurretCount();
    PlayerTank = Cast<APawn_Tank>(UGameplayStatics::GetPlayerPawn(this, 0));
    playerControllerRef = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));

    GameStart();

    if (playerControllerRef){
        playerControllerRef->SetPlayerEnabledState(false);

        FTimerHandle playerEnableHandle;
        FTimerDelegate playerEnableDelegate = FTimerDelegate::CreateUObject(playerControllerRef,
                                                                            &APlayerControllerBase::SetPlayerEnabledState,
                                                                            true);
        GetWorld()->GetTimerManager().SetTimer(playerEnableHandle, playerEnableDelegate, StartDelay, false);
    }
}

int32 ATankGameModeBase::GetTargetTurretCount() {
    TArray<AActor*> TurretActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnTurret::StaticClass(), TurretActors);
    return TurretActors.Num();
}

void ATankGameModeBase::HandleGameOver(bool PlayerWon)
{
    GameOver(PlayerWon);
}

void ATankGameModeBase::ActorDied(AActor* DeadActor)
{
    if (DeadActor == PlayerTank) {
        PlayerTank->HandleDestruction();
        UE_LOG(LogTemp, Warning, TEXT("YOU DIED!"))
        UE_LOG(LogTemp, Warning, TEXT("%i turrets left"), TargetTurrets)
        HandleGameOver(false);

        if (playerControllerRef){
            playerControllerRef->SetPlayerEnabledState(false);
        }
    }
    else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor)) {
        DestroyedTurret->HandleDestruction();

        if (--TargetTurrets == 0) {
            UE_LOG(LogTemp, Warning, TEXT("YOU WON!"))
            HandleGameOver(true);
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("%i turrets remaining"), TargetTurrets)
        }
    }
}