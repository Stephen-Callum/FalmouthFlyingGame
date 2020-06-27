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

protected:

	virtual void BeginPlay() override;

private:
	APawn* PawnToChase;

	UFUNCTION()
	void FollowPawn();
	UFUNCTION()
	float PawnToChasePitch(APawn* PawnToChase);

	UFUNCTION()
	float PawnToChaseYawRoll(APawn* PawnToChase);
};
