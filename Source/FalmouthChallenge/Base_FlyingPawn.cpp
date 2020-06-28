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

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
}

void ABase_FlyingPawn::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Deflect off surface that the pawn collides with
	const FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
}

// Called every frame
void ABase_FlyingPawn::Tick(float DeltaTime)
{

	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaTime, 0.0f, 0.0f);

	//// Move pawn forwards, sweeping stops the pawn when we collide with other objects
	AddActorLocalOffset(LocalMove, true);

	// Calculate change in rotation this frame
	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaTime;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaTime;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaTime;

	// Rotate pawn
	AddActorLocalRotation(DeltaRotation);

	GEngine->AddOnScreenDebugMessage(0, 0.0f, FColor::Cyan, FString::Printf(TEXT("ForwardSpeed: %f"), CurrentForwardSpeed));

	Super::Tick(DeltaTime);

}

void ABase_FlyingPawn::SpeedInput(float Val)
{
	// Check if there is any input
	const bool bHasInput = !FMath::IsNearlyEqual(Val, 0.0f);

	float CurrentAcc = 0.0f;

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
	float TargetYawSpeed = (Val * YawRateMultiplier);

	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.0f);

	bIntendingRoll = FMath::Abs(Val) > 0.0f;

	if (bIntendingPitch && !bIntendingRoll) return;

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


