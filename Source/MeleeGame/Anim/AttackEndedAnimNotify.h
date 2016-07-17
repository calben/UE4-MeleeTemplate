// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "AttackEndedAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class MELEEGAME_API UAttackEndedAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
	
public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
