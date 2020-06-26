// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Base_FlyingPawn.generated.h"

UCLASS()
class FALMOUTHCHALLENGE_API ABase_FlyingPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABase_FlyingPawn();

protected:

	// Static mesh component for bird
	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* PawnMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// Bound to speedup action
	UFUNCTION()
	void SpeedInput(float Val);

	// Bound to yaw/roll of pawn. Multiply by rate and pass on to MoveByYawRoll
	UFUNCTION()
	void YawRollInput(float Rate);

	// Bound to pitch of pawn. Multiply by rate and pass on to MoveByPitch
	UFUNCTION()
	void PitchInput(float Rate);

	// Add rotation to pawn via roll/yaw
	UFUNCTION()
	void MoveByYawRoll(float Val);

	// Add rotation onto pawn via pitch
	UFUNCTION()
	void MoveByPitch(float Val);

	/** How quickly forward speed changes */
	UPROPERTY(Category = Plane, EditAnywhere)
	float Acceleration;

	/** How quickly pawn can steer */
	UPROPERTY(Category = Plane, EditAnywhere)
	float TurnSpeed;

	/** How quickly pawn can steer */
	UPROPERTY(Category = Plane, EditAnywhere)
	float YawRateMultiplier;
	
	/** How quickly pawn can steer */
	UPROPERTY(Category = Plane, EditAnywhere)
	float RollRateMultiplier;

	/** How quickly pawn can steer */
	UPROPERTY(Category = Plane, EditAnywhere)
	float PitchRateMultiplier;

	/** Max forward speed */
	UPROPERTY(Category = Pitch, EditAnywhere)
	float MaxSpeed;

	/** Min forward speed */
	UPROPERTY(Category = Yaw, EditAnywhere)
	float MinSpeed;

	/** Current forward speed */
	float CurrentForwardSpeed;

	/** Current yaw speed */
	float CurrentYawSpeed;

	/** Current pitch speed */
	float CurrentPitchSpeed;

	/** Current roll speed */
	float CurrentRollSpeed;

	bool bIntendingPitch{ false };

	bool bIntendingRoll{ false };

public:

	// Return mesh subobject
	FORCEINLINE class UStaticMeshComponent* GetPlaneMesh() const { return PawnMesh; }

};
