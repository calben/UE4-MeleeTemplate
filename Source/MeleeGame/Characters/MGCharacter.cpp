// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeGame.h"
#include "MGCharacter.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AMGCharacter::AMGCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.1f;
	GetCharacterMovement()->GravityScale = 1.4f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	EyesArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("EyesArrow"));
	EyesArrow->SetupAttachment(RootComponent);
	EyesArrow->SetRelativeLocation(FVector::FVector(0.0f, 0.0f, 50.0f));
}

// Called when the game starts or when spawned
void AMGCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFocusing)
	{
		if (LastTimeLookedTimer > LookAndMoveTimerThreshold)
		{
			if (LastTimeMovedTimer > 0.5)
				Controller->SetControlRotation(FMath::RInterpTo(Controller->GetControlRotation(),
					GetCurrentFocusingDirection(), DeltaTime, 2.0f));
			else
				Controller->SetControlRotation(FMath::RInterpTo(Controller->GetControlRotation(),
					GetCurrentFocusingDirection(), DeltaTime, 10.0f));
		}
	}
	LastTimeLookedTimer += DeltaTime;
	LastTimeMovedTimer += DeltaTime;
}

// Called to bind functionality to input
void AMGCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("Focus", IE_Pressed, this, &AMGCharacter::OnFocusButton);

	InputComponent->BindAction("Defend", IE_Pressed, this, &AMGCharacter::OnDefendPressed);
	InputComponent->BindAction("Defend", IE_Released, this, &AMGCharacter::OnDefendReleased);

	InputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AMGCharacter::OnPrimaryAttackPressed);

	InputComponent->BindAxis("MoveForward", this, &AMGCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMGCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AMGCharacter::LookRightRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AMGCharacter::LookUpRate);
}


void AMGCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		FVector Direction;
		if (bIsFocusing)
		{
			// use focus direction as forward
			const FRotator YawRotation(0, GetCurrentFocusingDirection().Yaw, 0);
			Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		}
		else
		{
			// use camera based right
			const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
			Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		}
		AddMovementInput(Direction, Value);
		LastTimeMovedTimer = 0.f;
	}
}


void AMGCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		FVector Direction;
		if (bIsFocusing)
		{
			// use focused direction as forward
			const FRotator YawRotation(0, GetCurrentFocusingDirection().Yaw, 0);
			Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		}
		else
		{
			// use camera based right
			const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);

			Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		}
		AddMovementInput(Direction, Value);
		LastTimeMovedTimer = 0.f;
	}
}


void AMGCharacter::LookUpRate(float Value)
{
	if (Value != 0.0f)
	{
		if (bIsFocusing)
		{
			AddControllerPitchInput(FocusingLookRateMultiplier * Value * GetWorld()->GetDeltaSeconds());
		}
		else
		{
			AddControllerPitchInput(Value * GetWorld()->GetDeltaSeconds());
		}
		LastTimeLookedTimer = 0.f;
	}
}


void AMGCharacter::LookRightRate(float Value)
{
	if (Value != 0.0f)
	{
		if (bIsFocusing)
		{
			AddControllerYawInput(FocusingLookRateMultiplier * Value * GetWorld()->GetDeltaSeconds());
		}
		else
		{
			AddControllerYawInput(Value * GetWorld()->GetDeltaSeconds());
		}
		LastTimeLookedTimer = 0.f;
	}
}


float AMGCharacter::GetCharacterMovementAngle()
{
	if (bIsFocusing)
	{
		FRotator delta = UKismetMathLibrary::MakeRotFromXY(GetVelocity(), GetVelocity()) -
			Controller->GetControlRotation();
		float angle = delta.Yaw;
		// clamp it so that backwards is never quite triggered
		return FMath::Clamp(angle, -179.0f, 179.0f);
	}
	else
	{
		return 0.0f;
	}
}

float AMGCharacter::GetCharacterMovementSpeed()
{
	return GetVelocity().Size();
}

bool AMGCharacter::GetCharacterJumping()
{
	return GetVelocity().Z ? true : false;
}

FHitResult AMGCharacter::GetTraceFromCamera()
{
	FHitResult f(ForceInit);
	FVector start = FollowCamera->GetComponentLocation();
	FVector direction = Controller->GetControlRotation().Vector();
	FCollisionQueryParams  params = FCollisionQueryParams(FName(TEXT("FocusTrace")), true, NULL);
	params.bTraceAsyncScene = true;
	start = start + (direction * 100.0f);
	FVector end = start + (direction * 2000.0f);
	GetWorld()->LineTraceSingleByChannel(f, start, end, ECC_Visibility, params);
	GetWorld()->DebugDrawTraceTag = "FocusTrace";
	return f;
}


void AMGCharacter::OnFocusButton()
{
	if (bIsFocusing)
	{
		SetFocus(false, nullptr);
	}
	else
	{
		SetFocus(true, GetTraceFromCamera().GetActor());
	}
}

void AMGCharacter::OnDefendPressed()
{
	bIsDefending = true;
	Controller->SetIgnoreMoveInput(true);
}

void AMGCharacter::OnDefendReleased()
{
	bIsDefending = false;
	Controller->SetIgnoreMoveInput(false);
}

void AMGCharacter::OnPrimaryAttackPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Primary Attacking"));
	float duration = PlayCombatAnimation(PrimaryAttack);
	GetWorldTimerManager().SetTimer(TimerHandle_OnCombatAnimationFinished, this, &AMGCharacter::StopCurrentCombatAnimation, duration, false);
}

void AMGCharacter::SetFocus(bool DoFocus, AActor* FocalPoint)
{
	if (DoFocus)
	{
		bIsFocusing = true;
		if (FocalPoint == nullptr)
		{
			FocusedDirection = GetControlRotation();
			FocusedActor = nullptr;
		}
		else
		{
			FocusedDirection = FRotator::ZeroRotator;
			FocusedActor = FocalPoint;
		}
		bUseControllerRotationYaw = true;
	}
	else
	{
		bIsFocusing = false;
		FocusedDirection = FRotator::ZeroRotator;
		FocusedActor = nullptr;
		bUseControllerRotationYaw = false;
	}
}


FRotator AMGCharacter::GetCurrentFocusingDirection()
{
	if (FocusedActor != nullptr)
	{
		return UKismetMathLibrary::FindLookAtRotation(EyesArrow->GetComponentLocation(), FocusedActor->GetActorLocation());;
	}
	else
	{
		return FocusedDirection;
	}
}


float AMGCharacter::PlayCombatAnimation(const FCombatAnimationStruct Animation)
{
	if (!Animation.Animation)
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL ANIMATION"));
		return 0.0f;
	}
	bIsCombatAnimating = true;
	PlayingAnimation = Animation;
	Controller->SetIgnoreMoveInput(true);
	PlayAnimMontage(Animation.Animation, Animation.AnimationSpeed);
	return Animation.Animation->GetPlayLength() / Animation.AnimationSpeed;
}

void AMGCharacter::StopCurrentCombatAnimation()
{
	UE_LOG(LogTemp, Warning, TEXT("FINISHED COMBAT ANIMATION"));
	bIsCombatAnimating = false;
	Controller->SetIgnoreMoveInput(false);
	StopAnimMontage(PlayingAnimation.Animation);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
}

