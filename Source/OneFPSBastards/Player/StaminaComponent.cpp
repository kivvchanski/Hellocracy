#include "StaminaComponent.h"

UStaminaComponent::UStaminaComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	CurrentStamina = MaxStamina;
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentStamina = MaxStamina;
}


void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float TimeSinceLastStaminaUse = GetWorld()->GetTimeSeconds() - LastStaminaConsumeTime;

	// Regenerate Stamina if not paused and delay has passed
	if (TimeSinceLastStaminaUse >= StaminaRegenDelay) {
		if (CurrentStamina < MaxStamina) {
			CurrentStamina += StaminaRegenRate * DeltaTime;
			if (CurrentStamina > MaxStamina) {
				CurrentStamina = MaxStamina;
			}
		}
	}

	bIsStaminaBarVisible =
		CurrentStamina < MaxStamina ||
		TimeSinceLastStaminaUse < StaminaBarVisibilityDuration;
}

bool UStaminaComponent::ConsumeStamina(float Amount)
{
	if (CurrentStamina >= Amount) {
		CurrentStamina -= Amount;
		LastStaminaConsumeTime = GetWorld()->GetTimeSeconds();
		return true;
	}
	return false;
}

