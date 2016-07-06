// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "MGCharacter.generated.h"

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

	void MoveForward(float Value);

	void MoveRight(float Value);

	void LookUpRate(float Value);

	void LookRightRate(float Value);

	void OnFocusButton();

	void SetFocus(bool DoFocus, AActor* FocalPoint);

	void OnDefendPressed();

	void OnDefendReleased();

	// for if the character is sidestepping
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Movement)
		float GetCharacterMovementAngle();

	// calculate real velocity of the player
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Movement)
		float GetCharacterMovementSpeed();

	FHitResult GetTraceFromCamera();

	FRotator GetCurrentFocusingDirection();
	
};
