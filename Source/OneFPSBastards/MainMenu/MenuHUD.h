// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"

class UserWidget;

UCLASS()
class ONEFPSBASTARDS_API AMenuHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	FORCEINLINE UUserWidget* GetMainMenuWidget() const { return MainMenuWidgetInstance; };

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

private:
	UPROPERTY()
	UUserWidget* MainMenuWidgetInstance;
};
