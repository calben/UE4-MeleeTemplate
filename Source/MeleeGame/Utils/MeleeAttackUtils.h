// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MeleeAttackUtils.generated.h"

USTRUCT()
struct FCombatAnimationStruct
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimSequence* Animation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DamageOnHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DamagReduction;
};

/**
 * 
 */
UCLASS()
class MELEEGAME_API UMeleeAttackUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
		
};
