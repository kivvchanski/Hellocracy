#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterHUD.generated.h"

class UProgressBar;

UCLASS()
class ONEFPSBASTARDS_API UCharacterHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable)
    void UpdateStaminaBar(float Percent);

protected:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* ProgressBar_Stamina;
};
