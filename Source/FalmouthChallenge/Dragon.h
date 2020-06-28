// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_FlyingPawn.h"
#include "Dragon.generated.h"

/**
 * 
 */
UCLASS()
class FALMOUTHCHALLENGE_API ADragon : public ABase_FlyingPawn
{
	GENERATED_BODY()

	ADragon();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	struct FVector DragonForwardVector;

	UPROPERTY(EditAnyWhere, Category = SkeletalMesh)
	class USkeletalMeshComponent* DragonSkeletalMesh;

protected:

	virtual void BeginPlay() override;

private:
	class APawn* PawnToChase;

	UFUNCTION()
	void FollowPawn();

	// Find the rotation to look at pawn
	UFUNCTION()
	FRotator FindPawnLookAtRotation();

	UFUNCTION()
	float PawnToChasePitch();

	UFUNCTION()
	float PawnToChaseYawRoll();

	void MoveByPitch(float DeltaTime);
	
	void MoveByYawRoll(float DeltaTime);

	void SpeedInput(float DeltaTime);
};
