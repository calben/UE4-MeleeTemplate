// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MeleeAttackUtils.generated.h"

USTRUCT()
struct FAttackAnimationStruct
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimSequence* animation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float damage;
};

/**
 * 
 */
UCLASS()
class MELEEGAME_API UMeleeAttackUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
		
};
