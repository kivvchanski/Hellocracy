#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include <Item/ABaseActiveItem.h>
#include "MainCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UStaminaComponent;
class UInventoryComponent;
class UCharacterHUD;
class USurfaceAudioData;

UCLASS()
class ONEFPSBASTARDS_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	AABaseActiveItem* ActiveWeapon;
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void SwapWeapon(AABaseActiveItem* NewWeapon);
	virtual void SwapWeapon_Implementation(AABaseActiveItem* NewWeapon);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaminaComponent* StaminaComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UInventoryComponent* InventoryComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UCharacterHUD* HUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* BaseItemAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AltItemAction;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

	void Interact();
	AActor* GetActorInView(float Distance);
	void ManageInteractableInView();

	void AlterCharacterRotationSpeed(float DeltaTime);
	void ManageSprint(float DeltaTime);
	void ManageSprintCamera(float DeltaTime);
	void ManageJump();
	void ManageHud();

	void JumpStarted(const FInputActionValue& Value);
	void JumpCompleted(const FInputActionValue& Value);
	void SprintStarted(const FInputActionValue& Value);
	void SprintCompleted(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USurfaceAudioData* FootstepDataAsset;
	bool IsPlayingFootstep = false;
	FTimerHandle FootstepTimer;
	UPROPERTY()
	class UAudioComponent* CurrentFootstepComp;

	USoundBase* GetFootstepSoundForSurface();
	void PlayFootstepSound();
	void ResetFootstep();
	void StopFootstepSound();
	void ManageFootstep();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void BaseItemActionStart();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void BaseItemActionStop();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void AltItemActionStart();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void AltItemActionStop();
	virtual void BaseItemActionStart_Implementation();
	virtual void BaseItemActionStop_Implementation();
	virtual void AltItemActionStart_Implementation();
	virtual void AltItemActionStop_Implementation();

private:
	bool bIsMoving = false;
	bool bIsJumpKeyDown = false;
	bool bWantsToSprint = false;
	bool bIsSprintBlocked = false;
	bool bIsSprinting = false;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float InteractionDistance = 250.0f;
	AActor* LastFocusedActor = nullptr; // for item outlining
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float GroundRotationRate = 240.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float AirRotationRate = GroundRotationRate * 0.4f;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float SprintRotationRate = GroundRotationRate * 0.7f;
	float TargetRotationRate = GroundRotationRate;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float RotationInterpSpeed = 5.0f; // how quickly rotation lerps

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float WalkSpeed = 400.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float SprintSpeed = WalkSpeed * 1.4f;

	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float SprintStaminaCost = 7.0f; // per second
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float JumpStaminaCost = 10.0f; // per jump

	float WalkArmLength = 0.0f;
	float WalkFOV = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	float FootstepIntervalWalk = 0.5f;
	float FootstepIntervalSprint = FootstepIntervalWalk / (SprintSpeed / WalkSpeed);

	// Camera sprint settings
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float SprintArmLength = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float SprintFOV = 97.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CameraInterpSpeed = 5.0f; // how quickly camera lerps
};
