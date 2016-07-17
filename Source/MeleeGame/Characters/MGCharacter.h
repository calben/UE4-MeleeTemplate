// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "MGCharacter.generated.h"

USTRUCT()
struct FCombatAnimationStruct
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* Animation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DamageOnHit = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DamagReduction = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AnimationSpeed = 1.0f;
};

UCLASS()
class MELEEGAME_API AMGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMGCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Health = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bIsDead = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		class UArrowComponent* EyesArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AActor* FocusedActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FRotator FocusedDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsFocusing;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float FocusingLookRateMultiplier = 0.5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float FocusingMoveRateMultiplier = 0.5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float LastTimeLookedTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float LastTimeMovedTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float LookAndMoveTimerThreshold;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool debug = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsDefending = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsCombatAnimating = false;


	////
	// ANIMATIONS
	//// 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCombatAnimationStruct PrimaryAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FCombatAnimationStruct PlayingAnimation;


	////
	// TIMERS
	////


	FTimerHandle TimerHandle_OnCombatAnimationFinished;


	////
	// ACTIONS
	////

	void MoveForward(float Value);

	void MoveRight(float Value);

	void LookUpRate(float Value);

	void LookRightRate(float Value);

	void OnFocusButton();

	void SetFocus(bool DoFocus, AActor* FocalPoint);

	void OnDefendPressed();

	void OnDefendReleased();

	void OnPrimaryAttackPressed();

	float PlayCombatAnimation(const FCombatAnimationStruct Animation);

	void StopCurrentCombatAnimation();

	void EquipItem(USkeletalMeshComponent* item);


	////
	// HELPER FUNCTIONS
	//// 

	// check if the character is sidestepping
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Movement)
		float GetCharacterMovementAngle();

	// calculate real velocity of the player
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Movement)
		float GetCharacterMovementSpeed();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Movement)
		bool GetCharacterJumping();

	FHitResult GetTraceFromCamera();

	FRotator GetCurrentFocusingDirection();

};
