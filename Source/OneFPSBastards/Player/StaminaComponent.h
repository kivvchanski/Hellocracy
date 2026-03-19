#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ONEFPSBASTARDS_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStaminaComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Returns true if successful
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	bool ConsumeStamina(float Amount);
	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetCurrentStamina() const { return CurrentStamina; }
	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetMaxStamina() const { return MaxStamina; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	bool bIsStaminaBarVisible = false;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float MaxStamina = 100.0f;
	UPROPERTY(VisibleAnywhere, Category = "Stamina")
	float CurrentStamina = MaxStamina;
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float StaminaRegenRate = 17.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float StaminaRegenDelay = 2.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float StaminaBarVisibilityDuration = 4.0f;

	// time since last stamina consumption
	float LastStaminaConsumeTime = -StaminaBarVisibilityDuration;
		
};
