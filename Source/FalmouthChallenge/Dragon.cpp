// Fill out your copyright notice in the Description page of Project Settings.


#include "Dragon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Math/TransformNonVectorized.h"

ADragon::ADragon()
{
    PrimaryActorTick.bCanEverTick = true;

    DragonSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DragonSkeletalMesh"));
    DragonSkeletalMesh->SetupAttachment(RootComponent);

    // Set flying control parameters
    Acceleration = 500.0f;
    TurnSpeed = 50.0f;
    MaxSpeed = 5000.0f;
    MinSpeed = 500.0f;
    CurrentForwardSpeed = 500.0f;
    YawRateMultiplier = 100.0f;
    PitchRateMultiplier = 100.0f;
    RollRateMultiplier = 100.0f;
}

void ADragon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //SpeedInput(DeltaTime);

    //MoveByPitch(DeltaTime);

    //MoveByYawRoll(DeltaTime);

    //const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaTime, 0.0f, 0.0f);

    //AddActorLocalOffset(LocalMove, true);

    //FRotator DeltaRotation(0.0f, 0.0f, 0.0f);
    //DeltaRotation.Pitch = CurrentPitchSpeed * DeltaTime;
    //DeltaRotation.Yaw = CurrentYawSpeed * DeltaTime;
    ////DeltaRotation.Roll = CurrentRollSpeed * DeltaTime;

    //AddActorLocalRotation(DeltaRotation);

    const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaTime, 0.0f, 0.0f);

    AddActorLocalOffset(LocalMove, true);

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

    PawnToChase = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
}

void ADragon::SpeedInput(float DeltaTime)
{
    const float CurrentAcc = DeltaTime * Acceleration;

    const float NewForwardSpeed = CurrentForwardSpeed + CurrentAcc;

    CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}

FRotator ADragon::FindPawnLookAtRotation()
{
    const auto RotationToPawn = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PawnToChase->GetActorLocation());

    return RotationToPawn;
}

void ADragon::MoveByPitch(float DeltaTime)
{
    
    
    //const auto PitchDifference = (FindPawnLookAtRotation().Pitch - GetActorRotation().Pitch);
    //
    //const bool bDragonShouldPitch = PitchDifference < 5.0f && PitchDifference > -5.0f;
    //
    //// Find pitch of pawn to chase
    //const auto TargetPitchSpeed = bDragonShouldPitch ? (FindPawnLookAtRotation().Pitch) : CurrentPitchSpeed;

    //CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, DeltaTime , 1.0f);
}

void ADragon::MoveByYawRoll(float DeltaTime)
{
    /*const auto TargetYawSpeed = FindPawnLookAtRotation().Yaw;

    CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, DeltaTime, 1.0f);

    const bool bIsTurning = (FindPawnLookAtRotation().Yaw - GetActorRotation().Yaw) != 0.0f;

    const auto TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetActorRotation().Roll * -2.0f);

    CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, DeltaTime, 1.0f);*/
}




