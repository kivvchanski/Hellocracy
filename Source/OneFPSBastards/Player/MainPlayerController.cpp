// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerController.h"
#include "EnhancedInputSubsystems.h"

void AMainPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Add the Input Mapping Context to the Local Player
    if (APlayerController* PlayerController = Cast<APlayerController>(this)) {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
            if (DefaultMappingContext) {
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }
        }
    }

    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);
    SetShowMouseCursor(false);
}
