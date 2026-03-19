// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IAlternateAction.generated.h"


UINTERFACE(MinimalAPI)
class UIAlternateAction : public UInterface
{
	GENERATED_BODY()
};


class ONEFPSBASTARDS_API IIAlternateAction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ItemActions")
	void StartAltAction();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ItemActions")
	void StopAltAction();


};
