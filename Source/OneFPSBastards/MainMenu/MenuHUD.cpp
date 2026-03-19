// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuHUD.h"

#include "Blueprint/UserWidget.h"

void AMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (!GetWorld() || !MainMenuWidgetClass) return;
	
	MainMenuWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);

	if (!MainMenuWidgetInstance) return;

	MainMenuWidgetInstance->AddToViewport();

	APlayerController* PC = GetOwningPlayerController();
	if (PC)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
	}
}
