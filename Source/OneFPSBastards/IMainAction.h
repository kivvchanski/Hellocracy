// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IMainAction.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIMainAction : public UInterface
{
	GENERATED_BODY()
};


class ONEFPSBASTARDS_API IIMainAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ItemActions")
	void StartMainAction();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ItemActions")
	void StopMainAction();
};
