// Fill out your copyright notice in the Description page of Project Settings.


#include "Dragon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"

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
    YawRateMultiplier = 200.0f;
    PitchRateMultiplier = 200.0f;
    RollRateMultiplier = 200.0f;
}

void ADragon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    SpeedInput(DeltaTime);

    MoveByPitch(DeltaTime);

    MoveByYawRoll(DeltaTime);

    const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaTime, 0.0f, 0.0f);

    AddActorLocalOffset(LocalMove, true);

    FRotator DeltaRotation(0.0f, 0.0f, 0.0f);
    DeltaRotation.Pitch = CurrentPitchSpeed * DeltaTime;
    DeltaRotation.Yaw = CurrentYawSpeed * DeltaTime;
    DeltaRotation.Roll = CurrentRollSpeed * DeltaTime;

    AddActorLocalRotation(DeltaRotation);
}

void ADragon::BeginPlay()
{
    Super::BeginPlay();

    PawnToChase = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
}

void ADragon::FollowPawn()
{
    // Use Base_FlyingPawn movement methods
    // if pawn is infront
        //dragon fly forward
    
    // if pawn is above/below dragon
        // dragon pitch to pawn
    // if pawn is to left/right of dragon
        // dragon yaw/roll to pawn

    
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

float ADragon::PawnToChasePitch()
{
    return 0.0f;
}

float ADragon::PawnToChaseYawRoll()
{
    return 0.0f;
}

void ADragon::MoveByPitch(float DeltaTime)
{
    // Find pitch of pawn to chase
    const auto TargetPitchSpeed = (FindPawnLookAtRotation().Pitch);

    CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, DeltaTime , 2.0f);
}

void ADragon::MoveByYawRoll(float DeltaTime)
{
    const auto TargetYawSpeed = FindPawnLookAtRotation().Yaw;

    CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, DeltaTime, 2.0f);

    float TargetRollSpeed = FindPawnLookAtRotation().Roll;

    CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, DeltaTime, 2.0f);
}




