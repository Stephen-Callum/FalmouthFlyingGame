// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_FlyingPawn.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"


// Sets default values
ABase_FlyingPawn::ABase_FlyingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create static mesh component
	PawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawnMesh"));

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
}

// Called when the game starts or when spawned
void ABase_FlyingPawn::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ABase_FlyingPawn::Tick(float DeltaTime)
{

	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaTime, 0.0f, 0.0f);

	// Move pawn forwards, sweeping stops the pawn when we collide with other objects
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

	// If there is input, multiply axis value by acceleration
	// If there is no input, reduce speed
	float CurrentAcc = bHasInput ? (Val * Acceleration) : (-0.5f * Acceleration);

	// Add/reduce accelaration based on pitch
	//CurrentAcc = CurrentAcc + (-GetActorRotation().Pitch * GetWorld()->GetDeltaSeconds() * Acceleration);

	// Calculate new speed
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);

	// Clamp between MinSpeed and MaxSpeed
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}

void ABase_FlyingPawn::YawRollInput(float Rate)
{
	if (FMath::Abs(Rate) > 0.2f)
	{
		MoveByYawRoll(Rate);
	}
}

void ABase_FlyingPawn::PitchInput(float Rate)
{
	if (FMath::Abs(Rate) > 0.2f)
	{
		MoveByPitch(Rate);
	}
}

void ABase_FlyingPawn::MoveByYawRoll(float Val)
{
	float TargetYawSpeed = (Val * TurnSpeed);

	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.0f);

	const bool bIsTurning = FMath::Abs(Val) > 0.2f;

	float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5) : (GetActorRotation().Roll * -2.0f);

	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.0f);

}

void ABase_FlyingPawn::MoveByPitch(float Val)
{
	// Is player intending to pitch?
	bIntendingPitch = FMath::Abs(Val) > 0.0f;

	// Define pitch speed to reach
	float TargetPitchSpeed = Val * TurnSpeed;

	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

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