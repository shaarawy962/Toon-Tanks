// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "Pawn_Tank.h"

APawnTurret::APawnTurret()
{

}

void APawnTurret::CheckFireCondition()
{
	// If player == null || player is destroyed then bail
	if (!PlayerPawn) 
	{
		return;
	}
	else
	{
		// if player is within range then fire
		if (ReturnDistanceToPlayer() <= 500.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Fire Condition Success"))
			// Fire
		}
	}


}


void APawnTurret::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(FireRateHandler, this, &APawnTurret::CheckFireCondition, FireRate, true);

	PlayerPawn = Cast<APawn_Tank>(UGameplayStatics::GetPlayerPawn(this, 0));
}


void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APawnTurret::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float APawnTurret::ReturnDistanceToPlayer()
{
	if (!PlayerPawn)
	{
		return 0.0f;
	}

	else
	{
		float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
		return Distance;
	}
}
