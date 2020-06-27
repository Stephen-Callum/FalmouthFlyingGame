// Fill out your copyright notice in the Description page of Project Settings.


#include "Dragon.h"
#include "Kismet/GameplayStatics.h"

ADragon::ADragon()
{
    PrimaryActorTick.bCanEverTick = true;

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

float ADragon::PawnToChasePitch(APawn* PawnToChase)
{
    return 0.0f;
}

float ADragon::PawnToChaseYawRoll(APawn* PawnToChase)
{
    return 0.0f;
}


