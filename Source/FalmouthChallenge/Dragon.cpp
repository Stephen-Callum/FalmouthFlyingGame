// Fill out your copyright notice in the Description page of Project Settings.


#include "Dragon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Math/TransformNonVectorized.h"

ADragon::ADragon()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create dragon skeletal mesh. Attach to root.
    DragonSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DragonSkeletalMesh"));
    DragonSkeletalMesh->SetupAttachment(RootComponent);

    // Set flying control parameters
    Acceleration = 500.0f;
    TurnSpeed = 50.0f;
    MaxSpeed = 3000.0f;
    MinSpeed = 500.0f;
    CurrentForwardSpeed = 500.0f;
    YawRateMultiplier = 50.0f;
    PitchRateMultiplier = 50.0f;
    RollRateMultiplier = 50.0f;
}

void ADragon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Move dragon forward
    const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaTime, 0.0f, 0.0f);
    AddActorLocalOffset(LocalMove, true);

    // Rotate dragon towards the player.
    const auto TargetRotation = UKismetMathLibrary::InverseTransformRotation(GetActorTransform(), FindPawnLookAtRotation());
    FRotator DeltaRotation(0.0f, 0.0f, 0.0f);
    DeltaRotation.Pitch = TargetRotation.Pitch * DeltaTime;
    DeltaRotation.Yaw = TargetRotation.Yaw * DeltaTime;
    DeltaRotation.Roll = TargetRotation.Roll * DeltaTime;
    AddActorLocalRotation(DeltaRotation);
}

void ADragon::BeginPlay()
{
    Super::BeginPlay();

    // Get player controlled pawn
    PawnToChase = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
}

void ADragon::SpeedInput(float DeltaTime)
{
    // Apply acceleration to dragon
    const float CurrentAcc = DeltaTime * (Acceleration);
    const float NewForwardSpeed = CurrentForwardSpeed + CurrentAcc;

    // Clamp speed between min and max speed
    CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}

FRotator ADragon::FindPawnLookAtRotation()
{
    // Find the look at rotation of dragon to player controlled pawn
    const auto RotationToPawn = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PawnToChase->GetActorLocation());

    return RotationToPawn;
}




