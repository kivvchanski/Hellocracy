#include "CharacterHUD.h"
#include "Components/ProgressBar.h"

void UCharacterHUD::UpdateStaminaBar(float Percent)
{
    if (ProgressBar_Stamina) {
        ProgressBar_Stamina->SetPercent(Percent);
    }
}