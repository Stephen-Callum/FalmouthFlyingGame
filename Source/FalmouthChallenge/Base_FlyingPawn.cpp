// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_FlyingPawn.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ABase_FlyingPawn::ABase_FlyingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create spherical collision for pawn
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;

	// Dissociate controller rotation from pawn.
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
}

void ABase_FlyingPawn::OnCollision(FVector HitNormal)
{
	const FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
	GEngine->AddOnScreenDebugMessage(0, 0.0f, FColor::Cyan, FString::Printf(TEXT("SOMETHING HIT")));
	CollisionTimer = 1.0f;
}

// Called every frame
void ABase_FlyingPawn::Tick(float DeltaTime)
{

	FVector LocalMove = FVector(0.0f, 0.0f, 0.0f);
	// If character is colliding with world, repel them backwards, else do normal forward input
	if (CollisionTimer > 0.0f)
	{
		LocalMove = FVector(-1000.0f * DeltaTime, 0.0f, 0.0f);
		CollisionTimer -= DeltaTime;
	}
	else
	{
		LocalMove = FVector(CurrentForwardSpeed * DeltaTime, 0.0f, 0.0f);
	}
	LocalMove = FVector(CurrentForwardSpeed * DeltaTime, 0.0f, 0.0f);

	//// Move pawn forwards, sweeping stops the pawn when we collide with other objects
	AddActorLocalOffset(LocalMove, true);

	// Calculate change in rotation this frame
	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaTime;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaTime;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaTime;

	// Rotate pawn
	AddActorLocalRotation(DeltaRotation);

	Super::Tick(DeltaTime);

}

void ABase_FlyingPawn::SpeedInput(float Val)
{
	// Check if there is any input
	const bool bHasInput = !FMath::IsNearlyEqual(Val, 0.0f);

	float CurrentAcc = 0.0f;

	// If player is facing down, add acceleration automatically, else do normal acceleration.
	if (bHasInput && GetActorRotation().Pitch > 0.0f)
	{
		CurrentAcc = GetWorld()->GetDeltaSeconds() * (Val * Acceleration);
	}
	else
	{
		CurrentAcc = GetActorRotation().Pitch < 0.0f ? (GetWorld()->GetDeltaSeconds() * Acceleration * 2.5f) : (GetWorld()->GetDeltaSeconds() * Acceleration) * -2.0f;
	}

	// Calculate new speed
	const float NewForwardSpeed = CurrentForwardSpeed + CurrentAcc;

	// Clamp between MinSpeed and MaxSpeed
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}

// Used to make checks and perform modifications on the player yaw/roll input
void ABase_FlyingPawn::YawRollInput(float Rate)
{
	MoveByYawRoll(Rate * 2.0f);
}

// Used to make checks and perform modifications on the player pitch input
void ABase_FlyingPawn::PitchInput(float Rate)
{
	MoveByPitch(Rate * 2.0f);
}

void ABase_FlyingPawn::MoveByYawRoll(float Val)
{
	// Interpolate current yaw to target yaw based on input
	float TargetYawSpeed = (Val * YawRateMultiplier);
	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.0f);

	// Is player rolling intentionally?
	bIntendingRoll = FMath::Abs(Val) > 0.0f;

	// If player is pitching and not rolling, exit function
	if (bIntendingPitch && !bIntendingRoll) return;

	// Interpolate current roll to target roll based on input
	float TargetRollSpeed = bIntendingRoll ? ((Val * RollRateMultiplier) + (CurrentYawSpeed * 0.5)) : (GetActorRotation().Roll * -2.0f);
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.0f);

}

void ABase_FlyingPawn::MoveByPitch(float Val)
{
	// Is player intending to pitch?
	bIntendingPitch = FMath::Abs(Val) > 0.0f;

	const float TargetPitchSpeed = Val * PitchRateMultiplier;

	// Smoothly interpolate to target pitch speed 
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.0f);
}

// Called to bind functionality to input
void ABase_FlyingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind controls to callback functions
	PlayerInputComponent->BindAxis("YawRollControl", this, &ABase_FlyingPawn::YawRollInput);
	PlayerInputComponent->BindAxis("PitchControl", this, &ABase_FlyingPawn::PitchInput);
	PlayerInputComponent->BindAxis("SpeedControl", this, &ABase_FlyingPawn::SpeedInput);
}


