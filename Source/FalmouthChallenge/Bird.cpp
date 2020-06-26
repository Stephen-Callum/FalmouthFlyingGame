// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // spring arm component creation and setup
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 160.0f;
    SpringArm->SocketOffset = FVector(0.0f, 0.0f, 60.0f);
    SpringArm->bEnableCameraLag = false;
    SpringArm->CameraLagSpeed = 15.0f;
    SpringArm->bInheritRoll = false;

    // Create and setup camera component
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;

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

void ABird::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}