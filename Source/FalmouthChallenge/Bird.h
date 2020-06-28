// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Base_FlyingPawn.h"
#include "Bird.generated.h"

UCLASS()
class FALMOUTHCHALLENGE_API ABird : public ABase_FlyingPawn
{
	GENERATED_BODY()

	

public:
	// Sets default values for this pawn's properties
	ABird();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Spring arm component for camera offset
	UPROPERTY(Category = Camera, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	// Over the shoulder camera component for bird
	UPROPERTY(Category = Camera, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	// Static mesh component for bird
	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* PawnSkeletalMesh;

	UPROPERTY(Category = TopDownCamera, EditAnywhere)
	class USpringArmComponent* TopDownSpringArm;

	UPROPERTY(Category = TopDownCamera, EditAnywhere, BlueprintReadWrite)
	class USceneCaptureComponent2D* TopDownCamera;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Change max speed depending on pitch
	void MaxSpeedModifier();
	

public:
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }

	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }

	// Return mesh subobject
	FORCEINLINE class USkeletalMeshComponent* GetSkeletalMesh() const { return PawnSkeletalMesh; }
};
