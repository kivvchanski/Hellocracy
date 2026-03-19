// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UBaseButton;

UCLASS()
class ONEFPSBASTARDS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TSoftObjectPtr<UWorld> GameLevel = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Buttons", meta = (BindWidget))
	UBaseButton* ButtonPlay;

	/*
	UPROPERTY(EditDefaultsOnly, Category = "Buttons", meta = (BindWidget))
	UBaseButton* ButtonSettings;
	*/

	UPROPERTY(EditDefaultsOnly, Category = "Buttons", meta = (BindWidget))
	UBaseButton* ButtonQuit;

	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	FORCEINLINE void OpenGameLevel();

	UFUNCTION()
	FORCEINLINE void QuitGame();
};
