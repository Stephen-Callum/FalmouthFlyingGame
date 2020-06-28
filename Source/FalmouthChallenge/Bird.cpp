// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // create static mesh component
    PawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawnMesh"));
    PawnMesh->SetupAttachment(RootComponent);

    // spring arm component creation and setup
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 2000.0f;
    SpringArm->SocketOffset = FVector(0.0f, 0.0f, 300.0f);
    SpringArm->bEnableCameraLag = false;
    SpringArm->CameraLagSpeed = 15.0f;
    SpringArm->bInheritRoll = false;

    // Create and setup camera component
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;

    TopDownSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TopDownSpringArm"));
    TopDownSpringArm->SetupAttachment(RootComponent);
    TopDownSpringArm->TargetArmLength = 2000.0f;
    TopDownSpringArm->bInheritRoll = false;
    TopDownSpringArm->bInheritPitch = false;

    TopDownCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("TopDownCamera"));
    TopDownCamera->SetupAttachment(TopDownSpringArm, USpringArmComponent::SocketName);

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

void ABird::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ABird::MaxSpeedModifier()
{
}
