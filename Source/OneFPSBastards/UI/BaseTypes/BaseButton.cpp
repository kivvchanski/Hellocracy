// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UObject/NoExportTypes.h"
#include "Styling/SlateColor.h"
#include "Styling/SlateTypes.h"

void UBaseButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!TextBlock_ButtonText) return;

	TextBlock_ButtonText->SetText(ButtonText);

	SetEnabled(bEnabled);

	if (!BaseButton) return;

	BaseButton->OnHovered.AddDynamic(this, &ThisClass::HandleOnHovered);
	BaseButton->OnUnhovered.AddDynamic(this, &ThisClass::HandleOnUnhovered);
}

void UBaseButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (!BaseButton) return;

	BaseButton->OnClicked.AddDynamic(this, &ThisClass::HandleOnClicked);
}

void UBaseButton::HandleOnClicked()
{
	TextBlock_ButtonText->SetColorAndOpacity(FSlateColor(PressedColor));

	OnBaseButtonClicked.Broadcast();
}

void UBaseButton::HandleOnHovered()
{
	TextBlock_ButtonText->SetColorAndOpacity(FSlateColor(HoveredColor));
}

void UBaseButton::HandleOnUnhovered()
{
	TextBlock_ButtonText->SetColorAndOpacity(FSlateColor(NormalColor));
}
