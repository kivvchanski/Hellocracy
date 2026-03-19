#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
    GENERATED_BODY()
};

class IInteractableInterface
{
    GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractionSystem")
	 void Interact(AMainCharacter* InteractingActor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractionSystem")
	 void OnFocusStart(AMainCharacter* InteractingActor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractionSystem")
	 void OnFocusEnd(AMainCharacter* InteractingActor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InteractionSystem")
	 void StopInteract(AMainCharacter* InteractingActor);
};
