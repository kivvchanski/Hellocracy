// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "BaseTypes/BaseButton.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ButtonPlay) return;
	ButtonPlay->OnBaseButtonClicked.AddDynamic(this, &ThisClass::OpenGameLevel);

	if (!ButtonQuit) return;
	ButtonQuit->OnBaseButtonClicked.AddDynamic(this, &ThisClass::QuitGame);
}

void UMainMenu::OpenGameLevel()
{
	if (!GetWorld()) return;
	if (!GameLevel.ToSoftObjectPath().IsValid()) return;

	UWorld* LevelToOpen = GameLevel.LoadSynchronous();
	if (!LevelToOpen) return;

	const FName LevelName = LevelToOpen->GetFName();

	UGameplayStatics::OpenLevel(this, LevelName);
}

void UMainMenu::QuitGame()
{
	if (!GetWorld()) return;
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}
