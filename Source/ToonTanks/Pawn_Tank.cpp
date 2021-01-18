// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn_Tank.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

APawn_Tank::APawn_Tank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

}

void APawn_Tank::CalculateMoveInput(float Value)
{
	MoveDirection = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);
}

void APawn_Tank::CalculateRotationInput(float Value)
{
	float RotateAmount = Value * RotateSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator Rotation = FRotator(0, RotateAmount, 0);
	RotationDirection = FQuat(Rotation);
}

void APawn_Tank::Move()
{
	AddActorLocalOffset(MoveDirection, true);
}

void APawn_Tank::Rotate()
{
	AddActorLocalRotation(RotationDirection, true);
}

void APawn_Tank::Fire() {
	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > FireRate;

	if (isReloaded) {
		Super::Fire();
		LastFireTime = FPlatformTime::Seconds();
	}
}


void APawn_Tank::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = Cast<APlayerController>(GetController());
}


void APawn_Tank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Rotate();
	Move();

	if (PlayerControllerRef)
	{
		FHitResult OutHitResult;
		PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, OutHitResult);
		FVector HitLocation = OutHitResult.ImpactPoint;
		RotateTurret(HitLocation);
	}
}

void APawn_Tank::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APawn_Tank::CalculateMoveInput);
	PlayerInputComponent->BindAxis("RotateRight", this, &APawn_Tank::CalculateRotationInput);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawn_Tank::Fire);
}

void APawn_Tank::HandleDestruction() {
	Super::HandleDestruction();
	UE_LOG(LogTemp, Warning, TEXT("%s is Destroyed"), *this->GetName())
	Destroy();
}