#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "StaminaComponent.h"
#include "InventoryComponent.h"
#include "UI/CharacterHUD.h"
#include "InteractableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SurfaceAudioData.h"
#include "Components/AudioComponent.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 0.0f; // Distance from player
	// Rotate the arm based on Controller (Mouse) rotation
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Camera does not need to rotate relative to arm
	FollowCamera->bUsePawnControlRotation = false;

	// This ensures the character rotates with the camera (Strafe behavior)
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Configure Character Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	GetCharacterMovement()->JumpZVelocity = 410.0f;
	GetCharacterMovement()->AirControl = 0.41f;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;


	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind Enhanced Input Actions
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (MoveAction) {
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		}

		if (LookAction) {
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
		}

		if (JumpAction) {
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMainCharacter::JumpStarted);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMainCharacter::JumpCompleted);
		}

		if (SprintAction) {
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AMainCharacter::SprintStarted);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMainCharacter::SprintCompleted);
		}

		if (InteractAction) {
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AMainCharacter::Interact);
		}
		if (BaseItemAction) {
			EnhancedInputComponent->BindAction(BaseItemAction, ETriggerEvent::Started, this, &AMainCharacter::BaseItemActionStart);
		}
		if (BaseItemAction) {
			EnhancedInputComponent->BindAction(BaseItemAction, ETriggerEvent::Completed, this, &AMainCharacter::BaseItemActionStop);
		}
		if (AltItemAction) {
			EnhancedInputComponent->BindAction(AltItemAction, ETriggerEvent::Started, this, &AMainCharacter::AltItemActionStart);
		}
		if (AltItemAction) {
			EnhancedInputComponent->BindAction(AltItemAction, ETriggerEvent::Completed, this, &AMainCharacter::AltItemActionStop);
		}

	}
}



void AMainCharacter::SwapWeapon_Implementation(AABaseActiveItem* NewWeapon)
{
	if (ActiveWeapon) {
		ActiveWeapon->OnUnEquip();
	}
	ActiveWeapon = NewWeapon;
	if (ActiveWeapon) {
		ActiveWeapon->OnEquip();
	}
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->RotationRate = FRotator(0.0f, GroundRotationRate, 0.0f);

	if (CameraBoom && FollowCamera) {
		WalkArmLength = CameraBoom->TargetArmLength;
		WalkFOV = FollowCamera->FieldOfView;
	}

	if (APlayerController* PC = Cast<APlayerController>(GetController())) {
		if (HUDWidgetClass) {
			HUDWidget = Cast<UCharacterHUD>(CreateWidget(PC, HUDWidgetClass));
			if (HUDWidget) {
				HUDWidget->AddToViewport();
				HUDWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ManageInteractableInView();

	AlterCharacterRotationSpeed(DeltaTime);
	ManageSprint(DeltaTime);
	ManageSprintCamera(DeltaTime);
	ManageJump(); // Allow jump while holding button

	ManageFootstep();

	ManageHud();
}
void AMainCharacter::Move(const FInputActionValue& Value)
{
	// Input is a Vector2D (X = Forward/Backward, Y = Right/Left)
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller) {
		// Find out which way is forward based on the Camera (Controller) rotation
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add Movement
		AddMovementInput(ForwardDirection, MovementVector.Y); // Y in Input is usually Forward (W/S)
		AddMovementInput(RightDirection, MovementVector.X);   // X in Input is usually Right (D/A)
	}
}
void AMainCharacter::Look(const FInputActionValue& Value)
{
	// Input is a Vector2D (X = Yaw, Y = Pitch)
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller) {
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
void AMainCharacter::Interact()
{
	AActor* HitActor = GetActorInView(InteractionDistance);
	if (!HitActor) return;
	if (HitActor->Implements<UInteractableInterface>()) {
		IInteractableInterface::Execute_Interact(HitActor, this);
	}
}
AActor* AMainCharacter::GetActorInView(float Distance)
{
	FVector Start;
	FRotator Rotation;
	GetController()->GetPlayerViewPoint(Start, Rotation);

	FVector End = Start + Rotation.Vector() * Distance;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	if (bHit) {
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->Implements<UInteractableInterface>()) {
			return HitActor;
		}
	}

	return nullptr;
}
void AMainCharacter::ManageInteractableInView()
{
	AActor* HitActor = GetActorInView(InteractionDistance);

	// Remove focus from previous actor
	if (LastFocusedActor && LastFocusedActor != HitActor) {
		if (LastFocusedActor->Implements<UInteractableInterface>()) {
			IInteractableInterface::Execute_OnFocusEnd(LastFocusedActor, this);
		}
		LastFocusedActor = nullptr;
	}

	// Apply focus to new actor
	if (HitActor && HitActor != LastFocusedActor) {
		if (HitActor->Implements<UInteractableInterface>()) {
			IInteractableInterface::Execute_OnFocusStart(HitActor, this);
			LastFocusedActor = HitActor;
		}
	}
}

void AMainCharacter::AlterCharacterRotationSpeed(float DeltaTime)
{
	if (!Controller) return;
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (!MovementComp) return;

	static const float AccelerationThreshold = 0.1f;
	bIsMoving = MovementComp->GetCurrentAcceleration().SizeSquared() > AccelerationThreshold;

	if (bIsMoving) {
		MovementComp->bOrientRotationToMovement = true;
	}
	else {
		MovementComp->bOrientRotationToMovement = false;
	}

	if (MovementComp->IsFalling()) {
		TargetRotationRate = AirRotationRate;
	}
	else if (bIsSprinting) {
		TargetRotationRate = SprintRotationRate;
	}
	else {
		TargetRotationRate = GroundRotationRate;
	}

	// Smoothly interpolate the current Yaw rotation rate towards the target rate
	MovementComp->RotationRate.Yaw = FMath::FInterpTo(
		MovementComp->RotationRate.Yaw,
		TargetRotationRate,
		DeltaTime,
		RotationInterpSpeed // Transition speed between rotation rates
	);
}
void AMainCharacter::ManageSprint(float DeltaTime)
{
	float TargetSpeed = WalkSpeed;

	// block until we have stamina for 2 second of sprint
	bool bHasStaminaForOneSecond = StaminaComponent->GetCurrentStamina() >= SprintStaminaCost * 2;
	if (bIsSprintBlocked && bHasStaminaForOneSecond) {
		bIsSprintBlocked = false;
	}

	bIsSprinting = 
		bWantsToSprint && bIsMoving && !GetCharacterMovement()->IsFalling() && !bIsSprintBlocked;

	if (bIsSprinting) {
		float StaminaToDrain = SprintStaminaCost * DeltaTime;

		if (StaminaComponent->ConsumeStamina(StaminaToDrain)) {
			TargetSpeed = SprintSpeed;
		}
		else {
			bIsSprintBlocked = true;
		}
	}

	GetCharacterMovement()->MaxWalkSpeed = TargetSpeed;
}
void AMainCharacter::ManageSprintCamera(float DeltaTime)
{
	if (!CameraBoom || !FollowCamera) return;

	float TargetArm = bIsSprinting ? SprintArmLength : WalkArmLength;
	float TargetFOV = bIsSprinting ? SprintFOV : WalkFOV;

	// smooth interpolate arm length
	CameraBoom->TargetArmLength = FMath::FInterpTo(
		CameraBoom->TargetArmLength,
		TargetArm,
		DeltaTime,
		CameraInterpSpeed
	);

	// smooth interpolate FOV
	FollowCamera->SetFieldOfView(
		FMath::FInterpTo(
			FollowCamera->FieldOfView,
			TargetFOV, 
			DeltaTime, 
			CameraInterpSpeed
		)
	);
}
void AMainCharacter::ManageJump()
{
	if (bIsJumpKeyDown && !GetCharacterMovement()->IsFalling()) {
		if (StaminaComponent->ConsumeStamina(JumpStaminaCost)) {
			Jump();
		}
	}
}
void AMainCharacter::ManageHud()
{
	if (HUDWidget && StaminaComponent) {
		HUDWidget->SetVisibility(
			StaminaComponent->bIsStaminaBarVisible ?
			ESlateVisibility::Visible :
			ESlateVisibility::Hidden
		);

		float Percent =
			StaminaComponent->GetCurrentStamina() / StaminaComponent->GetMaxStamina();
		HUDWidget->UpdateStaminaBar(Percent);
	}
}

void AMainCharacter::JumpStarted(const FInputActionValue& Value)
{
	bIsJumpKeyDown = true;
}
void AMainCharacter::JumpCompleted(const FInputActionValue& Value)
{
	// The key change: When the jump input stops (key is released), clear the flag.
	bIsJumpKeyDown = false;
	StopJumping();
}
void AMainCharacter::SprintStarted(const FInputActionValue& Value)
{
	bWantsToSprint = true;
}
void AMainCharacter::SprintCompleted(const FInputActionValue& Value)
{
	bWantsToSprint = false;
}

void AMainCharacter::ManageFootstep()
{
	const float CharacterSpeed = GetVelocity().Size2D();
	const bool bIsOnGround = !GetCharacterMovement()->IsFalling();

	if (CharacterSpeed > 50.0f && bIsOnGround) {
		if (!IsPlayingFootstep) {
			PlayFootstepSound();
		}
	}
	else {
		// We only call Stop if the audio is actually making noise
		if (CurrentFootstepComp && CurrentFootstepComp->IsPlaying()) {
			StopFootstepSound();
		}
	}
}
void AMainCharacter::PlayFootstepSound()
{
	USoundBase* Footstep = GetFootstepSoundForSurface();
	if (Footstep) {
		if (CurrentFootstepComp && CurrentFootstepComp->IsPlaying()) {
			CurrentFootstepComp->FadeOut(0.1f, 0.0f);
		}

		// Choose interval based on sprint state
		if (bIsSprinting) {
			FootstepIntervalSprint = FootstepIntervalWalk / (SprintSpeed / WalkSpeed);
		}
		float CurrentFootstepInterval = bIsSprinting ? FootstepIntervalSprint : FootstepIntervalWalk;
		
		//float PitchRandomization = 0.07f * FootstepIntervalWalk / CurrentFootstepInterval;
		// Pitch / Playback Rate
		//float Pitch = FMath::FRandRange(1.0f - PitchRandomization, 1.0f + PitchRandomization);
		float BasePlaybackRate = FootstepIntervalWalk / CurrentFootstepInterval;
		float Pitch = BasePlaybackRate * FMath::FRandRange(0.96f, 1.04f);

		CurrentFootstepComp = UGameplayStatics::SpawnSoundAtLocation(
            this, 
            Footstep, 
            GetActorLocation(),
            FRotator::ZeroRotator,
            FMath::FRandRange(0.93f, 1.07f), // Volume
			Pitch
        );

		IsPlayingFootstep = true;

		GetWorldTimerManager().SetTimer(
			FootstepTimer,
			this,
			&AMainCharacter::ResetFootstep,
			CurrentFootstepInterval,
			false
		);
	}
}
void AMainCharacter::StopFootstepSound()
{
	if (CurrentFootstepComp && CurrentFootstepComp->IsPlaying()) {
		CurrentFootstepComp->FadeOut(0.1f, 0.0f);
	}

	// Clear the timer so a new footstep doesn't trigger while stopping
	GetWorldTimerManager().ClearTimer(FootstepTimer);
	ResetFootstep();
}
void AMainCharacter::ResetFootstep()
{
	IsPlayingFootstep = false;
}
USoundBase* AMainCharacter::GetFootstepSoundForSurface()
{
	if (!FootstepDataAsset) return nullptr;

	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, 150.0f);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.bReturnPhysicalMaterial = true; // Required to detect surface type

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params)) {
		// Get the Surface Type from the Physical Material
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
		return FootstepDataAsset->GetSoundForSurface(SurfaceType);
	}

	return nullptr;
}

void AMainCharacter::BaseItemActionStart_Implementation()
{
	IIMainAction::Execute_StartMainAction(ActiveWeapon);
	UE_LOG(LogTemp, Warning, TEXT("I'm trying to atack"));
}
void AMainCharacter::BaseItemActionStop_Implementation()
{
	IIMainAction::Execute_StopMainAction(ActiveWeapon);
}

void AMainCharacter::AltItemActionStart_Implementation()
{
	IIAlternateAction::Execute_StartAltAction(ActiveWeapon);
}
void AMainCharacter::AltItemActionStop_Implementation()
{
	IIAlternateAction::Execute_StopAltAction(ActiveWeapon);
}

