// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeGame.h"
#include "AttackEndedAnimNotify.h"



void UAttackEndedAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UE_LOG(LogTemp, Warning, TEXT("ANIMATION FROM MESH: %s "), *MeshComp->GetName());
}
