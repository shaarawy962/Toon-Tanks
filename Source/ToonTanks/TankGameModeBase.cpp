// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"
#include "Pawn_Tank.h"
#include "PawnTurret.h"
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

    GameStart();
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
        HandleGameOver(false);
    }
    else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor)) {
        DestroyedTurret->HandleDestruction();

        if (--TargetTurrets == 0) {
            UE_LOG(LogTemp, Warning, TEXT("YOU WON!"))
            HandleGameOver(true);
        }
    }
}